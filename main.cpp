// Used Cpp 20 and Visual Studio 2019

#include <windows.h>
#include <gdiplus.h>
#include <stdio.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

// Source: [1]
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
 * Writes PNG
 *
 * @param path
 * @return
 */

// Source: [1] [2]
int convert(const fs::path * path) {

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID encoderClsid;
    Status stat;
    Image* image = new Image(path->c_str());

    // Get the CLSID of the PNG encoder.
    GetEncoderClsid(L"image/png", &encoderClsid);

    // Generate Bitmap to print to
    int width = 1900;
    int height = 1472;
    Bitmap* target = new Bitmap(width, height);
    Graphics* g = Graphics::FromImage(target);
    g->DrawImage(image, 0, 0, width, height);
    // end

    // Build output path
    fs::path bird = fs::path("PNGs");
    bird /= path->string();
    bird.replace_extension(".png");
    // Build output
    //std::wstring wstr = L"Bird\\";
    //std::wstring bla = wstr + path->c_str();
    std::cout << bird.string() << '\n';

    // Check whether target folders exists and create path
    fs::path directories = fs::path(bird);
    directories.remove_filename();
    if (!fs::exists(directories.c_str())) {
        fs::create_directories(directories.c_str());
    }

    // Creates or overwrites file!
    stat = target->Save(bird.c_str(), &encoderClsid, NULL);

    if(stat == Ok)
        printf("Bird.png was saved successfully\n");
    else
        printf("Failure: stat = %d\n", stat);

    delete image;
    GdiplusShutdown(gdiplusToken);
    return 0;
}

int main() {
    std::string path(".");
    // Only convert files with 'wmf' extension
    std::string ext(".wmf");

    // Loop recursively through folders and recognize all files with given file extension
    for (auto &p : fs::recursive_directory_iterator(path)) {
        if (p.path().extension() == ext) {
            std::cout << p.path().stem().string() << '\n';

            // copy the constant path
            //fs::path variablePath = p.path();

            convert(&p.path());
        }
    }
    return 0;
}

// Sources:
// [1] https://docs.microsoft.com/en-us/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
// [2] https://stackoverflow.com/questions/12098924/how-do-i-resize-shrink-an-emf-metafile-in-net

