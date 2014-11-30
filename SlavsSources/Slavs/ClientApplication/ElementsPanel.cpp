#include "stdafx.h"

#include "ElementsPanel.h"

#include "ElementsBox.h"

namespace
  {

  struct BoxComparer
    {
    const int m_box_id;
    BoxComparer(int i_box_id)
      : m_box_id(i_box_id)
      {}

    bool operator () (const std::unique_ptr<UI::ElementsBox>& ip_box)
      {
      return ip_box->GetId() == m_box_id;
      }
    };

  }

namespace UI
  {

  ElementsPanel::ElementsPanel()
    : mp_current_box(nullptr)
    , m_next_id(0)
    , m_simoltaneously_shown_elements(5)
    , m_offset(0)
    {
    SetSimoltaneouslyShownElements(5);
    }

  ElementsPanel::~ElementsPanel()
    {}

  void ElementsPanel::SetSimoltaneouslyShownElements(size_t i_elements_number)
    {
    m_simoltaneously_shown_elements = i_elements_number;
    m_current_shown_boxes.reserve(i_elements_number);
    }

  ElementsBox* ElementsPanel::AddBox(int i_type_id)
    {
    m_element_boxes.push_back(ElementPtr(new ElementsBox(*this, i_type_id, m_next_id)));
    ++m_next_id;
    ElementsBox* p_box = m_element_boxes.back().get();
    if (m_current_shown_boxes.size() < m_simoltaneously_shown_elements)
      m_current_shown_boxes.push_back(p_box);
    if (m_element_boxes.size() == 1)
      mp_current_box = p_box;
    return p_box;
    }

  bool ElementsPanel::CanShiftRght() const
    {
    const size_t max_shown_number = m_offset + m_simoltaneously_shown_elements;
    return max_shown_number < m_element_boxes.size();
    }

  bool ElementsPanel::CanShiftLeft() const
    {
    return m_offset > 0;
    }

  bool ElementsPanel::ShiftRight()
    {
    if (!CanShiftLeft())
      return false;
    --m_offset;
    UpdateCachedBoxes();
    return true;
    }

  bool ElementsPanel::ShiftLeft()
    {
    if (!CanShiftRght())
      return false;
    ++m_offset;
    UpdateCachedBoxes();
    return true;
    }

  void ElementsPanel::UpdateCachedBoxes()
    {
    m_current_shown_boxes.clear();
    const size_t last_index = m_offset + m_simoltaneously_shown_elements;
    for (size_t i = m_offset; i < last_index; ++i)
      m_current_shown_boxes.push_back(m_element_boxes.at(i).get());
    }

  bool ElementsPanel::SetCurrentBox(int i_box_id)
    {
    if (mp_current_box && mp_current_box->GetId() == i_box_id)
      return false;
    auto it = std::find_if(m_element_boxes.begin(), m_element_boxes.end(), BoxComparer(i_box_id));
    if (it == m_element_boxes.end())
      {
      assert(false);
      return false;
      }
    if (mp_current_box && mp_current_box->IsActive())
      mp_current_box->Deactivate();
    mp_current_box = it->get();
    mp_current_box->Activate();
    return true;
    }

  } // UI