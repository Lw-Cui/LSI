#ifndef GI_IMAGE_H_H
#define GI_IMAGE_H_H

#include <memory>
#include <string>

class ImageImpl;

class Image {
public:
    Image(int length, int weight);

    void save(const char *const filename);

    void set(int x, int y, float value);

private:
    std::shared_ptr<ImageImpl> impl;
};

#endif //GI_IMAGE_H_H
