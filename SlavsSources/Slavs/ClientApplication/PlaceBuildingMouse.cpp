#include "stdafx.h"

#include "PlaceBuildingMouse.h"

#include "Application.h"
#include "MouseManager.h"
#include "MouseTypes.h"
#include "Task.h"

#include "InputManager.h"

#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubMesh.h>
#include <OGRE/OgreRenderTarget.h>

namespace
  {

  const std::string PLANE_NAME = "PB_PLANE";

  void _CreateColourCubeMesh()
    {
    using namespace Ogre;
    static bool initialized = false;
    if (initialized)
      return;
    initialized = true;
    using namespace Ogre;
    /// Create the mesh via the MeshManager
    MeshPtr msh = MeshManager::getSingleton().createManual("ColourCube", "General");

    /// Create one submesh
    SubMesh* sub = msh->createSubMesh();

    const float sqrt13 = 0.577350269f; /* sqrt(1/3) */

    /// Define the vertices (8 vertices, each consisting of 2 groups of 3 floats
    const size_t nVertices = 8;
    const size_t vbufCount = 3 * 2 * nVertices;

    float vertices[vbufCount] = {
      -100.0, 100.0, -100.0,        //0 position
      -sqrt13, sqrt13, -sqrt13,     //0 normal
      100.0, 100.0, -100.0,         //1 position
      sqrt13, sqrt13, -sqrt13,      //1 normal
      100.0, -100.0, -100.0,        //2 position
      sqrt13, -sqrt13, -sqrt13,     //2 normal
      -100.0, -100.0, -100.0,       //3 position
      -sqrt13, -sqrt13, -sqrt13,    //3 normal
      -100.0, 100.0, 100.0,         //4 position
      -sqrt13, sqrt13, sqrt13,      //4 normal
      100.0, 100.0, 100.0,          //5 position
      sqrt13, sqrt13, sqrt13,       //5 normal
      100.0, -100.0, 100.0,         //6 position
      sqrt13, -sqrt13, sqrt13,      //6 normal
      -100.0, -100.0, 100.0,        //7 position
      -sqrt13, -sqrt13, sqrt13,     //7 normal
      };

    RenderSystem* rs = Root::getSingleton().getRenderSystem();
    RGBA colours[nVertices];
    RGBA *pColour = colours;
    // Use render system to convert colour value since colour packing varies
    rs->convertColourValue(ColourValue(1.0, 0.0, 0.0), pColour++); //0 colour
    rs->convertColourValue(ColourValue(1.0, 1.0, 0.0), pColour++); //1 colour
    rs->convertColourValue(ColourValue(0.0, 1.0, 0.0), pColour++); //2 colour
    rs->convertColourValue(ColourValue(0.0, 0.0, 0.0), pColour++); //3 colour
    rs->convertColourValue(ColourValue(1.0, 0.0, 1.0), pColour++); //4 colour
    rs->convertColourValue(ColourValue(1.0, 1.0, 1.0), pColour++); //5 colour
    rs->convertColourValue(ColourValue(0.0, 1.0, 1.0), pColour++); //6 colour
    rs->convertColourValue(ColourValue(0.0, 0.0, 1.0), pColour++); //7 colour

    /// Define 12 triangles (two triangles per cube face)
    /// The values in this table refer to vertices in the above table
    const size_t ibufCount = 36;
    unsigned short faces[ibufCount] = {
      0, 2, 3,
      0, 1, 2,
      1, 6, 2,
      1, 5, 6,
      4, 6, 5,
      4, 7, 6,
      0, 7, 4,
      0, 3, 7,
      0, 5, 1,
      0, 4, 5,
      2, 7, 3,
      2, 6, 7
      };

    /// Create vertex data structure for 8 vertices shared between submeshes
    msh->sharedVertexData = new VertexData();
    msh->sharedVertexData->vertexCount = nVertices;

    /// Create declaration (memory format) of vertex data
    VertexDeclaration* decl = msh->sharedVertexData->vertexDeclaration;
    size_t offset = 0;
    // 1st buffer
    decl->addElement(0, offset, VET_FLOAT3, VES_POSITION);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    decl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
    offset += VertexElement::getTypeSize(VET_FLOAT3);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    HardwareVertexBufferSharedPtr vbuf =
      HardwareBufferManager::getSingleton().createVertexBuffer(
      offset, msh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

    /// Set vertex buffer binding so buffer 0 is bound to our vertex buffer
    VertexBufferBinding* bind = msh->sharedVertexData->vertexBufferBinding;
    bind->setBinding(0, vbuf);

    // 2nd buffer
    offset = 0;
    decl->addElement(1, offset, VET_COLOUR, VES_DIFFUSE);
    offset += VertexElement::getTypeSize(VET_COLOUR);
    /// Allocate vertex buffer of the requested number of vertices (vertexCount) 
    /// and bytes per vertex (offset)
    vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
      offset, msh->sharedVertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    /// Upload the vertex data to the card
    vbuf->writeData(0, vbuf->getSizeInBytes(), colours, true);

    /// Set vertex buffer binding so buffer 1 is bound to our colour buffer
    bind->setBinding(1, vbuf);

    /// Allocate index buffer of the requested number of vertices (ibufCount) 
    HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().
      createIndexBuffer(
      HardwareIndexBuffer::IT_16BIT,
      ibufCount,
      HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    /// Upload the index data to the card
    ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

    /// Set parameters of the submesh
    sub->useSharedVertices = true;
    sub->indexData->indexBuffer = ibuf;
    sub->indexData->indexCount = ibufCount;
    sub->indexData->indexStart = 0;

    /// Set bounding information (for culling)
    msh->_setBounds(AxisAlignedBox(-100, -100, -100, 100, 100, 100));
    msh->_setBoundingSphereRadius(Math::Sqrt(3 * 100 * 100));

    /// Notify -Mesh object that it has been loaded
    msh->load();

    /* MaterialPtr material = MaterialManager::getSingleton().create(
    "Test/ColourTest", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    material->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_AMBIENT);*/
    }

  Ogre::SceneNode* CreatePlane()
    {
    _CreateColourCubeMesh();
    auto p_scene_manager = ClientGame::appInstance.GetOgreFramework().GetSceneManager();

    Ogre::Entity* p_cube_entity = nullptr;
    if (p_scene_manager->hasEntity(PLANE_NAME))
      p_cube_entity = p_scene_manager->getEntity(PLANE_NAME);
    else
      {
      p_cube_entity = p_scene_manager->createEntity(PLANE_NAME, "ColourCube");
      p_cube_entity->setMaterialName("Test/ColourTest");
      }
    
    Ogre::SceneNode* p_cube_node = p_scene_manager->getRootSceneNode()->createChildSceneNode();
    p_cube_node->setPosition(-35, 0, 0);
    p_cube_node->attachObject(p_cube_entity);

    p_cube_node->setInheritScale(true);
    p_cube_node->setScale(Ogre::Vector3(10.f, 0.5f, 10.f));
    p_cube_node->setPosition(Ogre::Vector3(1000.f, -50.f, 1000.f));
    return p_cube_node;
    }

  Ogre::Vector3 GetWorldPoint(float mouseX, float mouseY)
    {
    auto& framework = ClientGame::appInstance.GetOgreFramework();
    
    const int w = framework.GetViewport()->getActualWidth();
    const int h = framework.GetViewport()->getActualHeight();
    Ogre::Ray cameraRay = framework.GetCamera()->getCameraToViewportRay(mouseX / w, mouseY / h);
    Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
    std::pair<bool, Ogre::Real> t = cameraRay.intersects(plane);
    if (t.first)
      return cameraRay.getPoint(t.second);
    return Ogre::Vector3();
    }
  }
 
namespace UI
  {

  PlaceBuildingMouse::PlaceBuildingMouse()
    : IMouse(static_cast<int>(MouseType::PlaceBuilding))
    , mp_node(nullptr)
    , mp_current_task(nullptr)
    {}

  PlaceBuildingMouse::~PlaceBuildingMouse()
    {}

  void PlaceBuildingMouse::ConfirmBuildingCreation()
    {
    assert(mp_node);
    assert(mp_current_task);
    Ogre::Vector3 pos = mp_node->getPosition();
    mp_current_task->Complete(pos);
    mp_current_task = nullptr;
    }

  void PlaceBuildingMouse::ActivateImpl(SDK::Task* ip_task)
    {
    auto& cursor = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor();
    m_was_cursor_visible = cursor.isVisible();
    
    if (m_was_cursor_visible)
      cursor.hide();

    mp_node = CreatePlane();
    mp_node->setVisible(true);

    cursor.setPosition(CEGUI::Vector2f(150.f, 150.f));

    mp_current_task = ip_task;

    ClientGame::appInstance.GetInputManager().AddSubscriber(this);
    }

  void PlaceBuildingMouse::DeactivateImpl()
    {
    if (m_was_cursor_visible)
      CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();

    if (mp_node)
      {
      mp_node->removeAndDestroyAllChildren();
      ClientGame::appInstance.GetOgreFramework().GetSceneManager()->destroySceneNode(mp_node);
      mp_node = nullptr;
      }
    ClientGame::appInstance.GetInputManager().RemoveSubscriber(this);
    if (mp_current_task)
      mp_current_task->Discard(false);
    mp_current_task = nullptr;
    }

  void PlaceBuildingMouse::Update(long i_elapsed_time)
    {

    }

  /////////////////////////////////////////////////////////////////////
  // InputListener

  bool PlaceBuildingMouse::KeyReleased(const OIS::KeyEvent &keyEventRef)
    {
    if (keyEventRef.key == OIS::KC_ESCAPE)
      ClientGame::appInstance.GetMouseManager().PopMouse();
    return true;
    }

  bool PlaceBuildingMouse::MouseMoved(const OIS::MouseEvent &evt)
    {
    if (mp_node == nullptr)
      return true;

    CEGUI::Vector2f cursor_position = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
    Ogre::Vector3 vec = GetWorldPoint(cursor_position.d_x, cursor_position.d_y);
    mp_node->setPosition(vec.x, 50, vec.z);
    return true;
    }

  bool PlaceBuildingMouse::MousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
    {
    return true;
    }

  bool PlaceBuildingMouse::MouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
    {
    if (id == OIS::MouseButtonID::MB_Right)
      {
      ConfirmBuildingCreation();
      ClientGame::appInstance.GetMouseManager().PopMouse();
      }
    return true;
    }

  } // UI