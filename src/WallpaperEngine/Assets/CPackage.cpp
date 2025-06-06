#include "CPackage.h"
#include "CAssetLoadException.h"
#include "CPackageLoadException.h"
#include "WallpaperEngine/Logging/CLog.h"

#include <sstream>
#include <utility>

using namespace WallpaperEngine::Assets;

class CPackageEntry {
  public:
    CPackageEntry (std::string filename, uint32_t offset, uint32_t length) :
        filename (filename),
        offset (offset),
        length (length) {}

    std::string filename;
    uint32_t offset;
    uint32_t length;
};

CPackage::CPackage (std::filesystem::path path) : m_path (std::move (path)) {
    this->init ();
}

std::shared_ptr<const uint8_t[]> CPackage::readFile (const std::filesystem::path& filename, uint32_t* length) const {
    const auto it = this->m_contents.find (filename);

    if (it == this->m_contents.end ())
        throw CAssetLoadException (filename, "Cannot find the file in the package");

    // set file length if required
    if (length != nullptr)
        *length = it->second->length;

    return it->second->content;
}

void CPackage::init () {
    FILE* fp = fopen (this->m_path.c_str (), "rb+");

    if (fp == nullptr)
        throw CPackageLoadException (this->m_path, std::to_string (errno));

    // first validate header
    this->validateHeader (fp);
    // header is okay, load everything into memory
    this->loadFiles (fp);

    fclose (fp);
}

char* CPackage::readSizedString (FILE* fp) {
    unsigned int length = 0;

    if (fread (&length, sizeof (unsigned int), 1, fp) != 1)
        sLog.exception ("Cannot read sized string length on file ", this->m_path);

    // account for 0 termination of the string
    length++;

    char* pointer = new char [length];
    memset (pointer, 0, length);

    // read only the string bytes so the last one in the memory is 0
    length--;

    // read data from file
    if (fread (pointer, sizeof (char), length, fp) != length)
        sLog.exception ("Not enough bytes to read string of length ", length, " on file ", this->m_path);

    return pointer;
}

uint32_t CPackage::readInteger (FILE* fp) {
    uint32_t output;

    if (fread (&output, sizeof (uint32_t), 1, fp) != 1)
        sLog.exception ("Not enough bytes to read an integer from file ", this->m_path);

    return output;
}

void CPackage::validateHeader (FILE* fp) {
    const char* pointer = this->readSizedString (fp);

    if (strncmp ("PKGV", pointer, 4) != 0) {
        std::stringstream msg;
        msg << "Expected PKGV indicator, found " << pointer;
        delete [] pointer;
        throw std::runtime_error (msg.str ());
    }

    // free memory
    delete [] pointer;
}

void CPackage::loadFiles (FILE* fp) {
    const uint32_t count = this->readInteger (fp);
    std::vector<CPackageEntry> list;

    for (uint32_t index = 0; index < count; index++) {
        // first read the filename
        char* filename = this->readSizedString (fp);
        uint32_t offset = this->readInteger (fp);
        uint32_t length = this->readInteger (fp);

        // add the file to the list
        list.emplace_back (filename, offset, length);
        // only free filename, the file's contents are stored in a map for a later use
        delete [] filename;
    }

    // get current baseOffset, this is where the files start
    const long baseOffset = ftell (fp);

    for (const auto& cur : list) {
        const long offset = cur.offset + baseOffset;

        // with all the data we can jump to the offset and read the content
        if (fseek (fp, offset, SEEK_SET) != 0)
            sLog.exception ("Cannot find file ", cur.filename, " from package ", this->m_path);

        // allocate memory for the file's contents and read it from the file
        std::shared_ptr<uint8_t[]> contents = std::shared_ptr<uint8_t[]>(new uint8_t [cur.length]);

        if (fread (contents.get(), cur.length, 1, fp) != 1) {
            sLog.exception ("Cannot read file ", cur.filename, " contents from package ", this->m_path);
        }

        // add the file to the map
        this->m_contents.insert_or_assign (cur.filename, std::make_unique<CFileEntry> (contents, cur.length));
    }
}