#include "svFileCreateProjectAction.h"

#include "svApplicationPluginActivator.h"
#include "svProjectCreate.h"
//#include "svProjectManager.h"

//#include <QFileDialog>
//#include <QMessageBox>
//#include <QApplication>

//#include <mitkSceneIO.h>
#include <mitkProgressBar.h>
#include <mitkStatusBar.h>
//#include <mitkNodePredicateNot.h>
//#include <mitkNodePredicateProperty.h>
//#include <mitkProperties.h>

//#include <mitkCoreObjectFactory.h>
//#include <mitkDataStorageEditorInput.h>
#include <mitkIDataStorageService.h>
//#include <berryIEditorPart.h>
//#include <berryIWorkbenchPage.h>
//#include <berryIWorkbenchWindow.h>
//#include <berryIPreferencesService.h>
//#include <berryPlatform.h>

svFileCreateProjectAction::svFileCreateProjectAction(berry::IWorkbenchWindow::Pointer window)
    : QAction(0)
    , m_Window(nullptr)
{
    this->Init(window.GetPointer());
}

svFileCreateProjectAction::svFileCreateProjectAction(berry::IWorkbenchWindow* window)
    : QAction(0)
    , m_Window(nullptr)
{
    this->Init(window);
}

void svFileCreateProjectAction::Init(berry::IWorkbenchWindow* window)
{
    m_Window = window;
    this->setText("&Create SV Project...");
    this->setToolTip("Create SimVascular Project");

    this->connect(this, SIGNAL(triggered(bool)), this, SLOT(Run()));
}

void svFileCreateProjectAction::Run()
{
    try
    {
        mitk::IDataStorageReference::Pointer dsRef;

        {
            ctkPluginContext* context = svApplicationPluginActivator::getContext();
            mitk::IDataStorageService* dss = 0;
            ctkServiceReference dsServiceRef = context->getServiceReference<mitk::IDataStorageService>();
            if (dsServiceRef)
            {
                dss = context->getService<mitk::IDataStorageService>(dsServiceRef);
            }

            if (!dss)
            {
                QString msg = "IDataStorageService service not available. Unable to save sv projects.";
                MITK_WARN << msg.toStdString();
                QMessageBox::warning(QApplication::activeWindow(), "Unable to save sv projects", msg);
                return;
            }

            // Get the active data storage (or the default one, if none is active)
            dsRef = dss->GetDataStorage();
            context->ungetService(dsServiceRef);
        }

        mitk::DataStorage::Pointer dataStorage = dsRef->GetDataStorage();

        svProjectCreate* pc=new svProjectCreate(dataStorage);
        pc->move(400,400);
        pc->show();
        pc->SetFocus();
    }
    catch (std::exception& e)
    {
        MITK_ERROR << "Exception caught during opening SV projects: " << e.what();
    }
}
