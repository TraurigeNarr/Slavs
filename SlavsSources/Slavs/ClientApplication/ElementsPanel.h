#ifndef __ELEMENTSPANEL_H__
#define __ELEMENTSPANEL_H__

namespace UI
  {
  class ElementsBox;
  
  /*
  ElementsPanel controls all element boxes that will be created
    Its position is set in cegui layout
    Command boxes created in run time depends on plugins that were defined on server side
    Maybe some default commands should be defined
  */
  class ElementsPanel
    {
    private:
      typedef std::unique_ptr<ElementsBox> ElementPtr;

    public:
      int                     m_next_id;
      std::vector<ElementPtr> m_element_boxes;

      ElementsBox*            mp_current_box;
      int                     m_current_box_id;
      std::vector<ElementsBox*> m_current_shown_boxes;

      // number of elements that will be displayed simultaneously
      size_t                  m_simoltaneously_shown_elements;
      size_t                  m_offset;

    private:
      void                UpdateCachedBoxes();

    public:
      ElementsPanel();
      virtual ~ElementsPanel();

      ElementsBox*        AddBox(int i_type_id);

      bool                CanShiftRght() const;
      bool                CanShiftLeft() const;

      bool                ShiftRight();
      bool                ShiftLeft();

      const ElementsBox*  GetCurrentBox() const;
      bool                SetCurrentBox(int i_box_id);

      void                SetSimoltaneouslyShownElements(size_t i_elements_number);
      const std::vector<ElementsBox*>& GetCurrentShownBoxes() const;
    };

  inline const ElementsBox* ElementsPanel::GetCurrentBox() const
    {
    return mp_current_box;
    }

  inline const std::vector<ElementsBox*>& ElementsPanel::GetCurrentShownBoxes() const
    {
    return m_current_shown_boxes;
    }

  } // UI

#endif