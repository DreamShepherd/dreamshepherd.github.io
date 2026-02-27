/***************************************************************
 * Name:      PCspec.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2014-12-26
 * Copyright:  ()
 * License:
 **************************************************************/
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <vector>
#include "OutPlot.h"
//#include "_DPoissonWX3Main.h"
#include "..\1Dpoi_type.h"

extern "C" void alertbox(char message[]);

/****************************************/
void alertbox(char message[]) {
/****************************************/

    wxString messg(message, wxConvUTF8);
	wxMessageDialog dialog(NULL,messg,wxT("Alert"),wxOK | wxICON_ERROR);
	dialog.ShowModal();
	return;
}


/****************************************/
int PlotPrep(struct global_type *pGlobals, struct flag_type *pFlags, OutPlot* plot, struct plotSetUp_type* plotSetUp) {
/****************************************/

	struct mesh_type *pMesh;
	char messg[256];

     std::vector<std::vector<double> > vectorList(plotSetUp->numPlotVars+1, std::vector<double>(plotSetUp->numPlotVars+1));

    pMesh= pGlobals->pTopMesh;

     do {
        vectorList[0].push_back(pMesh->meshcoord);// depth axis first
        for (int i=0; i<plotSetUp->numPlotVars; i++) {
            if (!strcmp(plotSetUp->plotLabels[i], "Ec"))vectorList[i+1].push_back(pMesh->v+pMesh->offset);
            else if (!strcmp(plotSetUp->plotLabels[i], "Ev"))vectorList[i+1].push_back(pMesh->v+pMesh->offset-pMesh->Eg);
            else if (!strcmp(plotSetUp->plotLabels[i], "EF"))vectorList[i+1].push_back(pMesh->Efn);
            else if (!strcmp(plotSetUp->plotLabels[i], "E"))vectorList[i+1].push_back((pMesh->pDownMesh->v-pMesh->v)*1.e8/pMesh->pDownMesh->dy);
            else if (!strcmp(plotSetUp->plotLabels[i], "n"))vectorList[i+1].push_back(pMesh->n);
            else if (!strcmp(plotSetUp->plotLabels[i], "p"))vectorList[i+1].push_back(pMesh->p);
            else if (!strcmp(plotSetUp->plotLabels[i], "Nd-Na"))vectorList[i+1].push_back(pMesh->fcharge);
            //else if (!strcmp(plotSetUp->plotLabels[i], "Na"))vectorList[i+1].push_back(pMesh->v+pMesh->offset-pMesh->Eg);
        }

    } while ((pMesh = pMesh->pDownMesh) != pGlobals->pBottomMesh);

        //strcpy(messg ,"In plotprep");
        //alertbox(messg);
    plot->SetVector(vectorList, plotSetUp);
    plot->Show(true);

    return 0;
}
