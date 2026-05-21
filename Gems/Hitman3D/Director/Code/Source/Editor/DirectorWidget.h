// ═══════════════════════════════════════════════════════════════
// DirectorWidget.h — The conversational UI panel in the editor
// Users type natural-language commands here.
// ═══════════════════════════════════════════════════════════════

#pragma once

#include <AzCore/Memory/SystemAllocator.h>

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include <Hitman3D/Director/DirectorBus.h>

namespace Hitman3D::Director
{
    class DirectorWidget
        : public QWidget
        , protected DirectorNotificationBus::Handler
    {
        Q_OBJECT

    public:
        AZ_CLASS_ALLOCATOR(DirectorWidget, AZ::SystemAllocator);

        explicit DirectorWidget(QWidget* parent = nullptr);
        ~DirectorWidget() override;

    protected:
        // DirectorNotificationBus
        void OnCommandAccepted(CommandId id, const ParsedIntent& intent) override;
        void OnCommandCompleted(CommandId id, const CommandResult& result) override;
        void OnCommandFailed(CommandId id, const AZStd::string& error) override;

    private slots:
        void OnSubmitClicked();
        void OnInputReturnPressed();

    private:
        void AppendToLog(const QString& text, const QString& color = "white");
        void SubmitCurrentInput();

        QVBoxLayout* m_layout = nullptr;
        QLabel* m_titleLabel = nullptr;
        QTextEdit* m_logView = nullptr;
        QLineEdit* m_inputField = nullptr;
        QPushButton* m_submitButton = nullptr;
    };
}
