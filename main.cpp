#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>

using namespace Gdiplus;

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * Helper Method from [1]
 *
 * @param format
 * @param pClsid
 * @return
 * Source: [1]
 */
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

/**
 * Converts the '.wmf' file at {@code inputPath} to a '.png' file at PNGs/{@code inputPath}
 *
 * @param inputPath The location of the '.wmf' file
 * @return 0
 * Source: [3] [2]
 */
int convert(const wchar_t* filename, int width, int height) {

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID encoderClsid;
    Status stat;
    Image *image = new Image(filename);

    // Get the CLSID of the PNG encoder.
    GetEncoderClsid(L"image/png", &encoderClsid); // Wished output format

    // Generate Bitmap to print to
    Bitmap *target = new Bitmap(width, height);
    Graphics *g = Graphics::FromImage(target);
    g->DrawImage(image, 0, 0, width, height);
    // end

    // Build output inputPath
    fs::path outputPath = fs::path("PNGs"); // Wished output folder
    outputPath /= filename;
    outputPath.replace_extension(".png"); // Wished output format

    // Checks, whether target folders exists (and creates inputPath)
    fs::path directories = fs::path(outputPath);
    directories.remove_filename();
    if (!fs::exists(directories.c_str())) {
        fs::create_directories(directories.c_str());
    }

    // Creates or overwrites file!
    stat = target->Save(outputPath.wstring().c_str(), &encoderClsid, NULL);

    if (stat == Ok)
        printf("%s was saved successfully at\n%s\n",
               outputPath.filename().string().c_str(),
               outputPath.string().c_str());
    else
        printf("Failure: stat = %d\n", stat);

    delete image;

    // This prevents 'Failure: stat = 2' after approximately 90 convertions
    delete target;
    delete g;
    //

    GdiplusShutdown(gdiplusToken);
    return 0;
}

int main(int argc, char* argv[]) {
    // START: Validate the input
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " target-folder width height" << std::endl;
        return 1; // error code
    }

    std::string targetFolder = argv[1];
    int width = std::stoi(argv[2]); // 1900
    int height = std::stoi(argv[3]); // 1472

    std::cout << "The input string is: " << targetFolder << std::endl;
    std::cout << "The width is: " << width << std::endl;
    std::cout << "The height is: " << height << std::endl;
    // END: Validate the input

    std::string path(targetFolder);
    // Only convert files with 'wmf' extension
    std::string ext(".wmf"); // Wished input format

    // Loop recursively through folders and recognize all files with given file extension
    for (const auto& p : fs::recursive_directory_iterator(path)) {
        if (p.path().extension() == ext) {
            std::wstring filePathW = p.path().wstring();
            const wchar_t* filePathWC = filePathW.c_str();

            convert(filePathWC, width, height);
            std::cout << p.path().string() << std::endl;
        }
    }

    return 0; // success
}

// Sources:
// [1] https://docs.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
// [2] https://stackoverflow.com/questions/12098924/how-do-i-resize-shrink-an-emf-metafile-in-net
// [3] https://learn.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-converting-a-bmp-image-to-a-png-image-use
