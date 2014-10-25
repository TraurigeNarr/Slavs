#ifndef SGameObject_h
#define SGameObject_h

#include "SlavsServerAPI.h"

#include "Game\IGameObject.h"

class IController;
class TiXmlElement;
class Vector2D;
class CommandData;
class Telegram;
class SGameContext;

class SLAVS_SERVER_EXPORT SGameObject : public IGameObject
  {
  friend class IObjectComposer;
  public:
    enum component_type
	  {
	  	c_t_dynamicObject,
	  	c_t_staticObject,
	  	c_t_human,
	  	c_t_storeHouse,
	  	c_t_manufacture,
	  	c_t_house,
	  	c_t_building,
	  };
	  SGameObject(SGameContext *pContext, long ID, int otype, int iMask = 0x00, const TiXmlElement* configElement = NULL, Vector2D* extraData = NULL, IController* owner = NULL);
	  virtual ~SGameObject();

	  virtual void	Init();
	  virtual void	TickPerformed();

	  IController*	GetOwner() const{ return m_pOwner; }
	  SGameContext*	GetContext() const { return m_pContext; }

	  bool			IsTagged()const	{ return m_bTag; }
	  void			Tag()			{ m_bTag = true; }
	  void			UnTag()			{ m_bTag = false; }

	  virtual bool	ProcessCommand(const CommandData& cData);
	  virtual bool	HandleMessage(const Telegram& msg);
	  bool			    HasComponent(component_type c_t) const { return (m_iFlags & c_t) == c_t; }

    /// takes ownage of component
    void          AddComponent (IComponent* ip_component);
    /// validate state of components
    /// each component checks its state and that all
    /// dependencies are present in object
    bool          ProbeComponents ();
  protected:
	  void			ParseElement(const TiXmlElement* configElement, Vector2D *extraData);
	  void			AddComponent(const TiXmlElement* componentElement, Vector2D *extraData);
	  //owner of the object, if the object is Environment then this pointer should be NULL
	  IController*	m_pOwner;

	  //this is a generic flag. 
	  bool			m_bTag;
	  SGameContext*	m_pContext;

	  int				m_iFlags;
  };

#endif