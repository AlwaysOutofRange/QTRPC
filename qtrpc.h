#ifndef QTRPCPLUGIN_H
#define QTRPCPLUGIN_H

#include "qtrpc_global.h"

#include <extensionsystem/iplugin.h>
#include <QString>
#include <discord_rpc.h>

namespace QTRPC {
namespace Internal {

class QTRPCPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QTRPC.json")

public:
    QTRPCPlugin();
    ~QTRPCPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    QString oldProject;
    void initDiscord();
};

} // namespace Internal
} // namespace QTRPC

#endif // QTRPCPLUGIN_H
