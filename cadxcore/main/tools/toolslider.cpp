/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "toolslider.h"

#if defined (ENABLE_SLIDERTOOL)
#include <export/contracts/iwidgetscontract.h>

#include <api/globals.h>
#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>
#include <vtkInteractorStyle.h>
#include <main/managers/widgetsmanager.h>
#include <api/ivista.h>
#include <main/controllers/controladorlog.h>


#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

namespace GNC
{
class EXTAPI wxToolSliderKeyHandler: public wxEvtHandler
{
public:
        wxToolSliderKeyHandler(wxEvtHandler* pParent, GNC::ToolSlider* pTool): wxEvtHandler()
        {
                m_pParent = pParent;
                m_pTool = pTool;
                // Connect Events
                m_pParent->Connect(wxEVT_CHAR, wxKeyEventHandler( wxToolSliderKeyHandler::OnChar),NULL,this);
        }

        virtual ~wxToolSliderKeyHandler()
        {
                m_pParent->Disconnect(wxEVT_CHAR, wxKeyEventHandler( wxToolSliderKeyHandler::OnChar),NULL,this);
        }

        virtual void OnChar(wxKeyEvent& event)
        {
                GNC::GCS::ISliderContract* SliderContract = m_pTool->SliderContract;
                switch(event.GetKeyCode()) {
                case WXK_RIGHT: {
                        if (SliderContract) {
                                const int maxSlices = SliderContract->GetNumberOfSlices();
                                const int newSlice = SliderContract->GetSlice() + 1;
                                if (newSlice >= maxSlices) {
                                        SliderContract->SetSlice(0);
                                } else {
                                        SliderContract->SetSlice( newSlice );
                                }
                        }
                        event.Skip(false);
                }
                break;
                case WXK_LEFT: {
                        if (SliderContract) {
                                const int newSlice = SliderContract->GetSlice() - 1;
                                if (newSlice >= 0) {
                                        SliderContract->SetSlice(newSlice);
                                } else {
                                        const int maxSlices = SliderContract->GetNumberOfSlices();
                                        SliderContract->SetSlice( maxSlices - 1 );
                                }
                        }
                        event.Skip(false);
                }
                break;
                case WXK_UP: {
                        if (SliderContract) {
                                SliderContract->GoToPrevSeries();
                        }
                        event.Skip(false);
                }
                break;
                case WXK_DOWN: {
                        if (SliderContract) {
                                SliderContract->GoToNextSeries();
                        }
                        event.Skip(false);
                }
                break;
                default:
                        event.ResumePropagation(10);
                        event.Skip(true);
                }
        }
protected:
        wxEvtHandler* m_pParent;
        GNC::ToolSlider* m_pTool;
};
}


GNC::ToolSlider::ToolSlider()
{
        m_pBuilder = NULL;
        m_pHandler = NULL;
}

GNC::ToolSlider::~ToolSlider()
{
        if (m_pBuilder != NULL) {
                delete m_pBuilder;
                m_pBuilder = NULL;
        }
        if (m_pHandler != NULL) {
                delete m_pHandler;
        }
}


GNC::GCS::ITool* GNC::ToolSlider::NewTool()
{
        return new GNC::ToolSlider();
}

void GNC::ToolSlider::Connect()
{
        if (SliderContract) {
                if (m_pBuilder != NULL) {
                        delete m_pBuilder;
                        m_pBuilder = NULL;
                }

                m_pBuilder = new TBuilder(SliderContract->GetManager(),SliderContract, GetTriggerButton(), (unsigned long)this);
                SliderContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
                SliderContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
        }
}

void GNC::ToolSlider::Disconnect()
{
        if (SliderContract) {
                SliderContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
                SliderContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
                // DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
                if (m_pBuilder != NULL) {
                        delete m_pBuilder;
                        m_pBuilder = NULL;
                }
        }
}

void GNC::ToolSlider::ConnectKeyEvents(wxWindow* pWindow)
{
        if (pWindow) {
                if (m_pHandler != NULL) {
                        LOG_WARN("ToolSlider", "Registering key events twice");
                        delete m_pHandler;
                }
                m_pHandler = new wxToolSliderKeyHandler(pWindow, this);
        }
}
//endregion
#endif
