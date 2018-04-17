#define cimg_display 0

// Attention: "3" means high debug messages
#define cimg_verbosity 3
#include <CImg.h>
#include <image.h>

using namespace std;

class ImageImpl {
public:
    ImageImpl(int length, int width) : image(length, width, 1, 3, 255) {
    }

    void save(const char *const filename) {
        image.save(filename);
    }

    void set(int x, int y, float value) {
        image(x, y, 0) = image(x, y, 1) = image(x, y, 2) = value;
    }

private:
    cimg_library::CImg<float> image;

};

Image::Image(int length, int width) : impl(make_shared<ImageImpl>(length, width)) {

}

void Image::save(const char *const filename) {
    impl->save(filename);
}

void Image::set(int x, int y, float value) {
    impl->set(x, y, value);
}
