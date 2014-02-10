//////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////

ALIGN(16, class) App
{
public:

	int					Run();
	void				Close();

	virtual void		OnInit() = 0;
	virtual bool		OnUpdate() = 0;
	virtual void		OnClose() = 0;

	static App &		Get();

protected:

	App();
	~App();
	
	uint32 pad[3];	// +1 for the vtable

};
