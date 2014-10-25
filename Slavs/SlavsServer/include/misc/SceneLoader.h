#ifndef SceneLoader_h
#define SceneLoader_h
//standard
#include <ostream>
#include <vector>
#include <string>

class SGameContext;
class TiXmlElement;

class SceneLoader
{
public:
	SceneLoader(SGameContext* context, std::ostream *stream = NULL);
	~SceneLoader();

	void LoadScene(std::string sceneName);
	SGameContext* GetContext() const { return m_pGameContext; }
protected:
	void Parse(TiXmlElement *rootElement);

	/*
		Parse node of xml-document and creates Object via SGameContext->AddObject()
	*/
	void ParseObject(const TiXmlElement* objectElement);
private:
	inline void WriteToStream(const std::string& message) const;
	//in this stream class writes log of loading
	std::ostream *m_pLogStream;
	SGameContext *m_pGameContext;

};

#endif