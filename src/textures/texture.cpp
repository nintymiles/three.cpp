//
// Created by SeanR on 2023/10/11.
//

size_t Texture::textureId = 0;
Texture::Texture(std::vector<unsigned char> image,TextureMapping mapping,Wrapping wraps,Wrapping wrapt,TextureFilter magFilter,TextureFilter minFilter, PixelFormat format,TextureDataType type, unsigned anisotropy, TextureEncoding encoding)
{
    id = textureId++;

    name = "";

    this->image = image;

    internalFormat = PixelFormatGPU::NONE;

    this->format = format;

    offset.set(0, 0);

    repeat.set(1, 1);

    center.set(0, 0);

    rotation = 0;

    matrixAutoUpdate = true;

    generateMipmaps = true;

    premultiplyAlpha = false;

    flipY = true;

    unpackAlignment = 4;

    version = 0;

    this->mapping = mapping;
    this->wrapS = wraps;
    this->wrapT = wrapt;
    this->magFilter = magFilter;
    this->minFilter = minFilter;
    this->format = format;
    this->type = type;
    this->anisotropy = anisotropy;
    this->encoding = encoding;
}
Texture::Texture(const Texture& source) {
    name = source.name;

    image = source.image;

    mipmaps = source.mipmaps;

    mapping = source.mapping;

    wrapS = source.wrapS;
    wrapT = source.wrapT;

    magFilter = source.magFilter;
    minFilter = source.minFilter;

    anisotropy = source.anisotropy;

    format = source.format;
    internalFormat = source.internalFormat;
    type = source.type;

    offset.copy(source.offset);
    repeat.copy(source.repeat);
    center.copy(source.center);
    rotation = source.rotation;

    //TODO copy images

    matrixAutoUpdate = source.matrixAutoUpdate;
    matrix.copy(source.matrix);

    generateMipmaps = source.generateMipmaps;
    premultiplyAlpha = source.premultiplyAlpha;
    flipY = source.flipY;
    unpackAlignment = source.unpackAlignment;
    encoding = source.encoding;
}
Texture* Texture::clone()
{
    return new Texture(*this);
}

Texture& Texture::copy(const Texture& source)
{
    name = source.name;

    image = source.image;

    mipmaps = source.mipmaps;

    mapping = source.mapping;

    wrapS = source.wrapS;
    wrapT = source.wrapT;

    magFilter = source.magFilter;
    minFilter = source.minFilter;

    anisotropy = source.anisotropy;

    format = source.format;
    internalFormat = source.internalFormat;
    type = source.type;

    offset.copy(source.offset);
    repeat.copy(source.repeat);
    center.copy(source.center);
    rotation = source.rotation;

    //TODO copy images

    matrixAutoUpdate = source.matrixAutoUpdate;
    matrix.copy(source.matrix);

    generateMipmaps = source.generateMipmaps;
    premultiplyAlpha = source.premultiplyAlpha;
    flipY = source.flipY;
    unpackAlignment = source.unpackAlignment;
    encoding = source.encoding;

    return *this;
}