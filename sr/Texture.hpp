//
//  SoftwareRenderer
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <climits>
#include <stdexcept>
#include <vector>
#include "Sampler.hpp"

namespace sr
{
    class Texture final
    {
    public:
        enum class PixelFormat
        {
            r8,
            a8,
            rgba8,
            float32
        };

        static constexpr float GAMMA = 2.2F;

        static std::uint32_t getPixelSize(PixelFormat pixelFormat) noexcept
        {
            switch (pixelFormat)
            {
                case PixelFormat::r8:
                case PixelFormat::a8:
                    return sizeof(std::uint8_t) * 1;
                case PixelFormat::rgba8:
                    return sizeof(std::uint8_t) * 4;
                case PixelFormat::float32:
                    return sizeof(float);
                default:
                    return 0;
            }
        }

        Texture(PixelFormat initPixelFormat = PixelFormat::rgba8,
                std::uint32_t initWidth = 0,
                std::uint32_t initHeight = 0,
                bool initMipMaps = false):
            pixelFormat(initPixelFormat),
            width(initWidth),
            height(initHeight),
            mipMaps(initMipMaps)
        {
            const std::uint32_t pixelSize = getPixelSize(pixelFormat);

            if (pixelSize > 0 && width > 0 && height > 0)
            {
                levels.push_back(std::vector<std::uint8_t>(width * height * pixelSize));

                if (mipMaps)
                {
                    std::uint32_t newWidth = width;
                    std::uint32_t newHeight = height;

                    while (newWidth > 1 || newHeight > 1)
                    {
                        newWidth >>= 1;
                        newHeight >>= 1;

                        if (newWidth < 1) newWidth = 1;
                        if (newHeight < 1) newHeight = 1;

                        levels.push_back(std::vector<std::uint8_t>(newWidth * newHeight * pixelSize));
                    }
                }
            }
        }

        void resize(std::uint32_t newWidth, std::uint32_t newHeight)
        {
            width = newWidth;
            height = newHeight;

            const std::uint32_t pixelSize = getPixelSize(pixelFormat);
            if (pixelSize == 0)
                throw std::runtime_error("Invalid pixel format");

            levels.clear();
            levels.push_back(std::vector<std::uint8_t>(width * height * pixelSize));

            if (mipMaps)
            {
                newWidth = width;
                newHeight = height;

                while (newWidth > 1 || newHeight > 1)
                {
                    newWidth >>= 1;
                    newHeight >>= 1;

                    if (newWidth < 1) newWidth = 1;
                    if (newHeight < 1) newHeight = 1;

                    levels.push_back(std::vector<std::uint8_t>(newWidth * newHeight * pixelSize));
                }
            }
        }

        PixelFormat getPixelFormat() const noexcept { return pixelFormat; }
        std::uint32_t getWidth() const noexcept { return width; }
        std::uint32_t getHeight() const noexcept { return height; }

        std::size_t getLevelCount() const noexcept
        {
            return levels.size();
        }

        std::vector<std::uint8_t>& getData(std::uint32_t level = 0)
        {
            return levels[level];
        }

        const std::vector<std::uint8_t>& getData(std::uint32_t level = 0) const
        {
            return levels[level];
        }

        void setData(const std::vector<std::uint8_t>& buffer, std::uint32_t level = 0)
        {
            const std::uint32_t pixelSize = getPixelSize(pixelFormat);
            if (pixelSize == 0)
                throw std::runtime_error("Invalid pixel format");

            if (buffer.size() != width * height * pixelSize)
                throw std::runtime_error("Invalid buffer size");

            if (level >= levels.size()) levels.resize(level + 1);
            levels[level] = buffer;
        }

        Color getPixel(std::uint32_t x, std::uint32_t y, std::uint32_t level) const
        {
            const std::vector<std::uint8_t>& buffer = levels[level];

            switch (pixelFormat)
            {
                case Texture::PixelFormat::r8:
                {
                    const std::uint8_t* r = &buffer[(y * width + x) * 1];
                    return sr::Color(*r, *r, *r, 255);
                }
                case Texture::PixelFormat::a8:
                {
                    const std::uint8_t* a = &buffer[(y * width + x) * 1];
                    return sr::Color(0, 0, 0, *a);
                }
                case Texture::PixelFormat::rgba8:
                {
                    const std::uint8_t* rgba = &buffer[(y * width + x) * 4];
                    return sr::Color(rgba[0], rgba[1], rgba[2], rgba[3]);
                }
                case Texture::PixelFormat::float32:
                {
                    float f = reinterpret_cast<const float*>(buffer.data())[y * width + x];
                    Color result;
                    result.r = result.g = result.b = f;
                    result.a = 1.0F;
                    return result;
                }
                default:
                    throw std::runtime_error("Invalid pixel format");
            }
        }

        void generateMipMaps()
        {
            const std::uint32_t pixelSize = getPixelSize(pixelFormat);
            if (pixelSize == 0)
                throw std::runtime_error("Invalid pixel format");

            if (levels.empty())
                throw std::runtime_error("Base image not provided");

            std::uint32_t newWidth = width;
            std::uint32_t newHeight = height;
            std::uint32_t previousWidth = width;
            std::uint32_t previousHeight = height;
            std::size_t level = 1;

            while (newWidth > 1 || newHeight > 1)
            {
                newWidth >>= 1;
                newHeight >>= 1;

                if (newWidth < 1) newWidth = 1;
                if (newHeight < 1) newHeight = 1;

                if (level >= levels.size())
                    levels.push_back(std::vector<std::uint8_t>(newWidth * newHeight * pixelSize));

                switch (pixelFormat)
                {
                    case PixelFormat::rgba8:
                        imageRGBA8Downsample2x2(previousWidth, previousHeight, previousWidth * 4,
                                                levels[level - 1].data(),
                                                levels[level].data());
                        break;
                    case PixelFormat::r8:
                        imageR8Downsample2x2(previousWidth, previousHeight, previousWidth * 1,
                                             levels[level - 1].data(),
                                             levels[level].data());
                    break;
                    case PixelFormat::a8:
                        imageA8Downsample2x2(previousWidth, previousHeight, previousWidth * 1,
                                             levels[level - 1].data(),
                                             levels[level].data());
                        break;
                    default:
                        throw std::runtime_error("Invalid pixel format");
                }

                previousWidth = newWidth;
                previousHeight = newHeight;
                ++level;
            }
        }

        Color sample(const Sampler* sampler, const Vector2F& coord) const
        {
            if (sampler && !levels.empty())
            {
                const float u =
                    (sampler->addressModeX == Sampler::AddressMode::clamp) ? clamp(coord.v[0], 0.0F, 1.0F) * (width - 1) :
                    (sampler->addressModeX == Sampler::AddressMode::repeat) ? std::fmod(coord.v[0], 1.0F) * (width - 1) :
                    (sampler->addressModeX == Sampler::AddressMode::mirror) ? 1.0F - 2.0F * std::fabs(std::fmod(coord.v[0] / 2.0F, 1.0F) - 0.5F) * (width - 1) :
                    0.0F;

                const float v =
                    (sampler->addressModeY == Sampler::AddressMode::clamp) ? clamp(coord.v[1], 0.0F, 1.0F) * (height - 1) :
                    (sampler->addressModeY == Sampler::AddressMode::repeat) ? std::fmod(coord.v[1], 1.0F) * (height - 1) :
                    (sampler->addressModeY == Sampler::AddressMode::mirror) ? 1.0F - 2.0F * std::fabs(std::fmod(coord.v[1] / 2.0F, 1.0F) - 0.5F) * (height - 1) :
                    0.0F;

                if (sampler->filter == Sampler::Filter::point)
                {
                    const std::uint32_t textureX = static_cast<std::uint32_t>(std::round(u));
                    const std::uint32_t textureY = static_cast<std::uint32_t>(std::round(v));
                    return getPixel(textureX, textureY, 0);
                }
                else
                {
                    std::uint32_t textureX0 = static_cast<std::uint32_t>(u - 0.5F);
                    std::uint32_t textureX1 = textureX0 + 1;
                    std::uint32_t textureY0 = static_cast<std::uint32_t>(v - 0.5F);
                    std::uint32_t textureY1 = textureY0 + 1;

                    textureX0 = clamp(textureX0, 0U, width - 1);
                    textureX1 = clamp(textureX1, 0U, width - 1);
                    textureY0 = clamp(textureY0, 0U, height - 1);
                    textureY1 = clamp(textureY1, 0U, height - 1);

                    // TODO: calculate mip level
                    Color color[4] = {
                        getPixel(textureX0, textureY0, 0),
                        getPixel(textureX1, textureY0, 0),
                        getPixel(textureX0, textureY1, 0),
                        getPixel(textureX1, textureY1, 0)
                    };

                    const float x0 = u - (textureX0 + 0.5F);
                    const float y0 = v - (textureY0 + 0.5F);
                    const float x1 = (textureX0 + 1.5F) - u;
                    const float y1 = (textureY0 + 1.5F) - v;

                    Color result;
                    result.r = color[0].r * x1 * y1 + color[1].r * x0 * y1 + color[2].r * x1 * y0 + color[3].r * x0 * y0;
                    result.g = color[0].g * x1 * y1 + color[1].g * x0 * y1 + color[2].g * x1 * y0 + color[3].g * x0 * y0;
                    result.b = color[0].b * x1 * y1 + color[1].b * x0 * y1 + color[2].b * x1 * y0 + color[3].b * x0 * y0;
                    result.a = color[0].a * x1 * y1 + color[1].a * x0 * y1 + color[2].a * x1 * y0 + color[3].a * x0 * y0;
                    return result;
                }
            }

            return Color();
        }

    private:
        static void imageA8Downsample2x2(std::uint32_t width, std::uint32_t height, std::uint32_t pitch, const std::uint8_t* src, std::uint8_t* dst)
        {
            const std::uint32_t dstWidth  = width >> 1;
            const std::uint32_t dstHeight = height >> 1;

            if (dstWidth > 0 && dstHeight > 0)
            {
                std::uint32_t y;
                std::uint32_t ystep;

                for (y = 0, ystep = pitch * 2; y < dstHeight; ++y, src += ystep)
                {
                    const std::uint8_t* pixel = src;
                    for (std::uint32_t x = 0; x < dstWidth; ++x, pixel += 2, dst += 1)
                    {
                        float a = 0.0F;
                        a += pixel[0];
                        a += pixel[1];
                        a += pixel[pitch + 0];
                        a += pixel[pitch + 1];
                        a /= 4.0F;
                        dst[0] = static_cast<std::uint8_t>(a);
                    }
                }
            }
            else if (dstHeight > 0)
            {
                std::uint32_t y;
                std::uint32_t ystep;

                for (y = 0, ystep = pitch * 2; y < dstHeight; ++y, src += ystep, dst += 1)
                {
                    const std::uint8_t* pixel = src;

                    float a = 0.0F;
                    a += pixel[0];
                    a += pixel[pitch + 0];
                    a /= 2.0F;
                    dst[0] = static_cast<std::uint8_t>(a);
                }
            }
            else if (dstWidth > 0)
            {
                const std::uint8_t* pixel = src;
                for (std::uint32_t x = 0; x < dstWidth; ++x, pixel += 2, dst += 1)
                {
                    float a = 0.0F;
                    a += pixel[0];
                    a += pixel[1];
                    a /= 2.0F;
                    dst[0] = static_cast<std::uint8_t>(a);
                }
            }
        }

        static void imageR8Downsample2x2(std::uint32_t width, std::uint32_t height, std::uint32_t pitch, const std::uint8_t* src, std::uint8_t* dst)
        {
            const std::uint32_t dstWidth  = width >> 1;
            const std::uint32_t dstHeight = height >> 1;

            if (dstWidth > 0 && dstHeight > 0)
            {
                std::uint32_t y;
                std::uint32_t ystep;

                for (y = 0, ystep = pitch * 2; y < dstHeight; ++y, src += ystep)
                {
                    const std::uint8_t* pixel = src;
                    for (std::uint32_t x = 0; x < dstWidth; ++x, pixel += 2, dst += 1)
                    {
                        float r = 0.0F;
                        r += std::pow(pixel[0] / 255.0F, GAMMA);
                        r += std::pow(pixel[1] / 255.0F, GAMMA);
                        r += std::pow(pixel[pitch + 0] / 255.0F, GAMMA);
                        r += std::pow(pixel[pitch + 1] / 255.0F, GAMMA);
                        r /= 4.0F;
                        dst[0] = static_cast<std::uint8_t>(std::round(std::pow(r, 1.0F / GAMMA) * 255.0F));
                    }
                }
            }
            else if (dstHeight > 0)
            {
                std::uint32_t y;
                std::uint32_t ystep;

                for (y = 0, ystep = pitch * 2; y < dstHeight; ++y, src += ystep, dst += 1)
                {
                    const std::uint8_t* pixel = src;

                    float r = 0.0F;
                    r += std::pow(pixel[0] / 255.0F, GAMMA);
                    r += std::pow(pixel[pitch + 0] / 255.0F, GAMMA);
                    r /= 2.0F;
                    dst[0] = static_cast<std::uint8_t>(std::round(std::pow(r, 1.0F / GAMMA) * 255.0F));
                }
            }
            else if (dstWidth > 0)
            {
                const std::uint8_t* pixel = src;
                for (std::uint32_t x = 0; x < dstWidth; ++x, pixel += 2, dst += 1)
                {
                    float r = 0.0F;
                    r += std::pow(pixel[0] / 255.0F, GAMMA);
                    r += std::pow(pixel[1] / 255.0F, GAMMA);
                    r /= 2.0F;
                    dst[0] = static_cast<std::uint8_t>(std::round(std::pow(r, 1.0F / GAMMA) * 255.0F));
                }
            }
        }

        static void imageRGBA8Downsample2x2(std::uint32_t width, std::uint32_t height, std::uint32_t pitch, const std::uint8_t* src, std::uint8_t* dst)
        {
            const std::uint32_t dstWidth  = width >> 1;
            const std::uint32_t dstHeight = height >> 1;

            if (dstWidth > 0 && dstHeight > 0)
            {
                std::uint32_t y;
                std::uint32_t ystep;
                for (y = 0, ystep = pitch * 2; y < dstHeight; ++y, src += ystep)
                {
                    const std::uint8_t* pixel = src;
                    for (std::uint32_t x = 0; x < dstWidth; ++x, pixel += 8, dst += 4)
                    {
                        float pixels = 0.0F;
                        float r = 0.0F;
                        float g = 0.0F;
                        float b = 0.0F;
                        float a = 0.0F;

                        if (pixel[3] > 0)
                        {
                            r += std::pow(pixel[0] / 255.0F, GAMMA);
                            g += std::pow(pixel[1] / 255.0F, GAMMA);
                            b += std::pow(pixel[2] / 255.0F, GAMMA);
                            pixels += 1.0F;
                        }
                        a += pixel[3];

                        if (pixel[7] > 0)
                        {
                            r += std::pow(pixel[4] / 255.0F, GAMMA);
                            g += std::pow(pixel[5] / 255.0F, GAMMA);
                            b += std::pow(pixel[6] / 255.0F, GAMMA);
                            pixels += 1.0F;
                        }
                        a += pixel[7];

                        if (pixel[pitch + 3] > 0)
                        {
                            r += std::pow(pixel[pitch + 0] / 255.0F, GAMMA);
                            g += std::pow(pixel[pitch + 1] / 255.0F, GAMMA);
                            b += std::pow(pixel[pitch + 2] / 255.0F, GAMMA);
                            pixels += 1.0F;
                        }
                        a += pixel[pitch + 3];

                        if (pixel[pitch + 7] > 0)
                        {
                            r += std::pow(pixel[pitch + 4] / 255.0F, GAMMA);
                            g += std::pow(pixel[pitch + 5] / 255.0F, GAMMA);
                            b += std::pow(pixel[pitch + 6] / 255.0F, GAMMA);
                            pixels += 1.0F;
                        }
                        a += pixel[pitch + 7];

                        if (pixels > 0.0F)
                        {
                            r /= pixels;
                            g /= pixels;
                            b /= pixels;
                            a *= 0.25F;
                            dst[0] = static_cast<std::uint8_t>(std::round(std::pow(r, 1.0F / GAMMA) * 255.0F));
                            dst[1] = static_cast<std::uint8_t>(std::round(std::pow(g, 1.0F / GAMMA) * 255.0F));
                            dst[2] = static_cast<std::uint8_t>(std::round(std::pow(b, 1.0F / GAMMA) * 255.0F));
                            dst[3] = static_cast<std::uint8_t>(a);
                        }
                        else
                        {
                            dst[0] = 0;
                            dst[1] = 0;
                            dst[2] = 0;
                            dst[3] = 0;
                        }
                    }
                }
            }
            else if (dstHeight > 0)
            {
                std::uint32_t y;
                std::uint32_t ystep;

                for (y = 0, ystep = pitch * 2; y < dstHeight; ++y, src += ystep, dst += 4)
                {
                    const std::uint8_t* pixel = src;

                    float pixels = 0.0F;
                    float r = 0.0F;
                    float g = 0.0F;
                    float b = 0.0F;
                    float a = 0.0F;

                    if (pixel[3] > 0)
                    {
                        r += std::pow(pixel[0] / 255.0F, GAMMA);
                        g += std::pow(pixel[1] / 255.0F, GAMMA);
                        b += std::pow(pixel[2] / 255.0F, GAMMA);
                        pixels += 1.0F;
                    }
                    a = pixel[3];

                    if (pixel[pitch + 3] > 0)
                    {
                        r += std::pow(pixel[pitch + 0] / 255.0F, GAMMA);
                        g += std::pow(pixel[pitch + 1] / 255.0F, GAMMA);
                        b += std::pow(pixel[pitch + 2] / 255.0F, GAMMA);
                        pixels += 1.0F;
                    }
                    a += pixel[pitch + 3];

                    if (pixels > 0.0F)
                    {
                        r /= pixels;
                        g /= pixels;
                        b /= pixels;
                        a *= 0.5F;
                        dst[0] = static_cast<std::uint8_t>(std::round(std::pow(r, 1.0F / GAMMA) * 255.0F));
                        dst[1] = static_cast<std::uint8_t>(std::round(std::pow(g, 1.0F / GAMMA) * 255.0F));
                        dst[2] = static_cast<std::uint8_t>(std::round(std::pow(b, 1.0F / GAMMA) * 255.0F));
                        dst[3] = static_cast<std::uint8_t>(a);
                    }
                    else
                    {
                        dst[0] = 0;
                        dst[1] = 0;
                        dst[2] = 0;
                        dst[3] = 0;
                    }
                }
            }
            else if (dstWidth > 0)
            {
                const std::uint8_t* pixel = src;
                for (std::uint32_t x = 0; x < dstWidth; ++x, pixel += 8, dst += 4)
                {
                    float pixels = 0.0F;
                    float r = 0.0F;
                    float g = 0.0F;
                    float b = 0.0F;
                    float a = 0.0F;

                    if (pixel[3] > 0)
                    {
                        r += std::pow(pixel[0] / 255.0F, GAMMA);
                        g += std::pow(pixel[1] / 255.0F, GAMMA);
                        b += std::pow(pixel[2] / 255.0F, GAMMA);
                        pixels += 1.0F;
                    }
                    a += pixel[3];

                    if (pixel[7] > 0)
                    {
                        r += std::pow(pixel[4] / 255.0F, GAMMA);
                        g += std::pow(pixel[5] / 255.0F, GAMMA);
                        b += std::pow(pixel[6] / 255.0F, GAMMA);
                        pixels += 1.0F;
                    }
                    a += pixel[7];

                    if (pixels > 0.0F)
                    {
                        r /= pixels;
                        g /= pixels;
                        b /= pixels;
                        a *= 0.5F;
                        dst[0] = static_cast<std::uint8_t>(std::round(std::pow(r, 1.0F / GAMMA) * 255.0F));
                        dst[1] = static_cast<std::uint8_t>(std::round(std::pow(g, 1.0F / GAMMA) * 255.0F));
                        dst[2] = static_cast<std::uint8_t>(std::round(std::pow(b, 1.0F / GAMMA) * 255.0F));
                        dst[3] = static_cast<std::uint8_t>(a);
                    }
                    else
                    {
                        dst[0] = 0;
                        dst[1] = 0;
                        dst[2] = 0;
                        dst[3] = 0;
                    }
                }
            }
        }

        PixelFormat pixelFormat;
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        bool mipMaps = false;
        std::vector<std::vector<std::uint8_t>> levels;
        std::uint32_t minLOD = 0;
        std::uint32_t maxLOD = UINT_MAX;
        float lodBias = 0.0F;
    };
}

#endif
