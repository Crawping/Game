//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

class Texture : public RefCount
{
public:

	static Texture *	Load(wchar const *filename);
	static Texture *	Create(uint width, uint height, Color color, bool createMips = false);
	
	void				Activate() const;	// uint channel?

	uint				Width() const;
	uint				Height() const;

private:

	Texture();
	~Texture();

	class TextureImpl;
	TextureImpl *pImpl;
};

//////////////////////////////////////////////////////////////////////

