// ═══════════════════════════════════════════════════════════════
// DirectorWidget.cpp — The Director conversational UI
// ═══════════════════════════════════════════════════════════════

#include "DirectorWidget.h"

#include <QFont>

namespace Hitman3D::Director
{
    DirectorWidget::DirectorWidget(QWidget* parent)
        : QWidget(parent)
    {
        m_layout = new QVBoxLayout(this);
        m_layout->setContentsMargins(8, 8, 8, 8);

        // Title
        m_titleLabel = new QLabel("Director — AI Assistant", this);
        QFont titleFont = m_titleLabel->font();
        titleFont.setPointSize(12);
        titleFont.setBold(true);
        m_titleLabel->setFont(titleFont);
        m_titleLabel->setStyleSheet("color: #00ccff;");
        m_layout->addWidget(m_titleLabel);

        // Log / conversation view
        m_logView = new QTextEdit(this);
        m_logView->setReadOnly(true);
        m_logView->setStyleSheet(
            "QTextEdit {"
            "  background-color: #1a1a2e;"
            "  color: #e0e0e0;"
            "  border: 1px solid #333;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-size: 10pt;"
            "}");
        m_layout->addWidget(m_logView, 1);

        AppendToLog("Welcome to Hitman 3d Director.", "#00ccff");
        AppendToLog("Tell me what you want to build. Type 'help' for examples.", "#888");

        // Input row
        auto* inputRow = new QWidget(this);
        auto* inputLayout = new QHBoxLayout(inputRow);
        inputLayout->setContentsMargins(0, 4, 0, 0);

        m_inputField = new QLineEdit(inputRow);
        m_inputField->setPlaceholderText("Tell me what to do...");
        m_inputField->setStyleSheet(
            "QLineEdit {"
            "  background-color: #16213e;"
            "  color: #fff;"
            "  border: 1px solid #00ccff;"
            "  padding: 6px;"
            "  font-size: 10pt;"
            "}");
        inputLayout->addWidget(m_inputField, 1);

        m_submitButton = new QPushButton("Go", inputRow);
        m_submitButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #00ccff;"
            "  color: #000;"
            "  font-weight: bold;"
            "  padding: 6px 16px;"
            "  border: none;"
            "}"
            "QPushButton:hover {"
            "  background-color: #00eeff;"
            "}");
        inputLayout->addWidget(m_submitButton);

        m_layout->addWidget(inputRow);

        connect(m_submitButton, &QPushButton::clicked, this, &DirectorWidget::OnSubmitClicked);
        connect(m_inputField, &QLineEdit::returnPressed, this, &DirectorWidget::OnInputReturnPressed);

        DirectorNotificationBus::Handler::BusConnect();
    }

    DirectorWidget::~DirectorWidget()
    {
        DirectorNotificationBus::Handler::BusDisconnect();
    }

    void DirectorWidget::AppendToLog(const QString& text, const QString& color)
    {
        m_logView->append(
            QString("<span style='color:%1;'>%2</span>").arg(color, text.toHtmlEscaped()));
    }

    void DirectorWidget::SubmitCurrentInput()
    {
        QString text = m_inputField->text().trimmed();
        if (text.isEmpty())
        {
            return;
        }

        AppendToLog(QString("> %1").arg(text), "#00ff88");
        m_inputField->clear();

        AZStd::string command(text.toUtf8().constData());
        DirectorBus::Broadcast(&DirectorBus::Events::SubmitCommand, command);
    }

    void DirectorWidget::OnSubmitClicked()
    {
        SubmitCurrentInput();
    }

    void DirectorWidget::OnInputReturnPressed()
    {
        SubmitCurrentInput();
    }

    void DirectorWidget::OnCommandAccepted(
        [[maybe_unused]] CommandId id,
        [[maybe_unused]] const ParsedIntent& intent)
    {
        // Could show a "thinking..." indicator here
    }

    void DirectorWidget::OnCommandCompleted(
        [[maybe_unused]] CommandId id,
        const CommandResult& result)
    {
        AppendToLog(QString::fromUtf8(result.message.c_str(), static_cast<int>(result.message.size())), "#00ccff");
        if (!result.detail.empty())
        {
            AppendToLog(
                QString::fromUtf8(result.detail.c_str(), static_cast<int>(result.detail.size())),
                "#666");
        }
    }

    void DirectorWidget::OnCommandFailed(
        [[maybe_unused]] CommandId id,
        const AZStd::string& error)
    {
        AppendToLog(
            QString::fromUtf8(error.c_str(), static_cast<int>(error.size())),
            "#ff4444");
    }
}

#include <Source/Editor/moc_DirectorWidget.cpp>
