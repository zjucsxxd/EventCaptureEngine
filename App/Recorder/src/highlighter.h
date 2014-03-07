/*Owner & Copyrights: Vance King Saxbe. A.*/#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>
#include "hunspell/hunspell.hxx"

class QTextDocument;

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0,QString SpellDic="",bool spellCheckState=false);
    ~Highlighter();
    void clearCheck();
    QStringList misspelled();
    QStringList correct();
    void enableSpellChecking(const bool state);
    void setSpellCheck(const bool state);
    bool setDict(const QString SpellDic);

public slots:
	void slot_addWord(QString word);

protected:
    void highlightBlock(const QString &text);
    void spellCheck(const QString &text);
    bool checkWord(QString word);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QString spell_dic, spell_encoding;
    Hunspell * pChecker;
    bool spellCheckActive,spellerError;
    QTextCharFormat spellCheckFormat;
    QTextCodec *codec;

};

#endif
/*email to provide support at vancekingsaxbe@powerdominionenterprise.com, businessaffairs@powerdominionenterprise.com, For donations please write to fundraising@powerdominionenterprise.com*/