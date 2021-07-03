#include "qtrpc.h"
#include "qtrpcconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

namespace QTRPC {
namespace Internal {

bool QTRPCPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    initDiscord();
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));

    presence.state = "Chilling";
    presence.details = "IDK What to code so yea";
    presence.largeImageKey = "qt";
    presence.largeImageText = "Just Started";
    presence.instance = 1;

    Discord_UpdatePresence(&presence);

    connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, [=](Core::IEditor *editor) {
       DiscordRichPresence presence;
       memset(&presence, 0, sizeof(presence));

       if(editor == nullptr) return;

       QString fileType = editor->document()->mimeType();

       const char *largeImageKey = "", *largeImageText;
       presence.smallImageKey = "qt";
       presence.smallImageText = "QT Creator";

       if(fileType == "text/x-c++hdr") {
           largeImageKey = "cpp";
           largeImageText = "C++ Header";
       } else if(fileType == "text/x-c++src") {
           largeImageKey = "cpp";
           largeImageText = "C++ File";
       } else if(fileType == "text/x-chdr") {
           largeImageKey = "c";
           largeImageText = "C Header";
       } else if(fileType == "text/x-csrc") {
           largeImageKey = "c";
           largeImageText = "C File";
       } else if(fileType == "application/x-designer") {
           largeImageKey = "creator";
           largeImageText = "QT Designer";
       } else if(fileType == "text/plain") {
           largeImageKey = "txt";
           largeImageText = "Plain Text";
       } else if(fileType == "application/json") {
           largeImageKey = "json";
           largeImageText = "Json File";
       } else if(fileType == "application/xml") {
           largeImageKey = "xml";
           largeImageText = "XML File";
       } else if(fileType == "text/vnd.qtcreator.git.submit") {
           largeImageKey = "git";
           largeImageText = "Git";
       } else if(fileType == "text/x-cmake-project") {
           largeImageKey = "cmake";
           largeImageText = "CMake File";
       } else {
           largeImageKey = "unknown";
           largeImageText = "Unknown File";
       }

       if(largeImageKey != nullptr && largeImageKey[0] == '\0') {
           presence.largeImageKey = "qt";
           presence.largeImageKey = "Working with QT Creator";
       } else {
           presence.largeImageKey = largeImageKey;
           presence.largeImageText = largeImageText;
       }

       ProjectExplorer::Project* current = ProjectExplorer::ProjectTree::currentProject();
       char state[256];

       if(current == nullptr) {
           if(editor->isDesignModePreferred()) {
                sprintf(state, "Designing UI");
           } else {
                 sprintf(state, "Editing a File");
           }
           oldProject = "";
       } else {
           sprintf(state, "%s", current->displayName().prepend("Working on ").toUtf8().data());

           if(oldProject != current->displayName()) {
                 oldProject = current->displayName();
              }
       }

       presence.state = state;

       char details[256];
       sprintf(details, "%s", editor->document()->filePath().fileName().prepend("Editing File ").toUtf8().data());

       presence.details = details;
       presence.instance = 1;

       Discord_UpdatePresence(&presence);

    });

    return true;
}

ExtensionSystem::IPlugin::ShutdownFlag QTRPCPlugin::aboutToShutdown()
{
    Discord_Shutdown();
    return SynchronousShutdown;
}

void QTRPCPlugin::initDiscord() {
    DiscordEventHandlers handler;

    memset(&handler, 0, sizeof(handler));

    handler.ready = [](const DiscordUser* user) {
        qInfo() << "RPC is ready";
    };

    handler.errored = [](int errorCode, const char* msg) {
        qInfo() << "ERROR";
    };

    handler.disconnected = [](int errorCode, const char* msg) {
        qInfo() << "Disconnect";
    };

    handler.joinGame = [](const char* joinSecret) {
        QByteArray secretBytes(joinSecret);
        QString secret = QString::fromStdString(secretBytes.toStdString());
        QMessageBox::warning(nullptr, "Discord Join", QString("Discord Join Secret: ").append(secret), QMessageBox::Ok, QMessageBox::Ok);
    };

    Discord_Initialize("860799807995117568", &handler, true, nullptr);
}

void QTRPCPlugin::extensionsInitialized(){}
QTRPCPlugin::~QTRPCPlugin(){}
QTRPCPlugin::QTRPCPlugin(){}

} // namespace Internal
} // namespace QTRPC
