#include "escape.h"
#include <QDebug>

static bool isHexDigit(const QChar ch)
{
    return (ch >= u'0' && ch <= u'9') ||
           (ch >= u'A' && ch <= u'F') ||
           (ch >= u'a' && ch <= u'f');
}

static int hexDigitValue(const QChar ch)
{
    if (ch >= u'0' && ch <= u'9') {
        return ch.unicode() - u'0';
    }
    if (ch >= u'a' && ch <= u'f') {
        return ch.unicode() - u'a' + 10;
    }
    if (ch >= u'A' && ch <= u'F') {
        return ch.unicode() - u'A' + 10;
    }
    return -1;
}

static bool isOctalDigit(const QChar ch)
{
    return (ch >= u'0' && ch <= u'7');
}

// https://github.com/abseil/abseil-cpp/blob/master/absl/strings/escaping.cc
QString cEscape(const QStringView src)
{
    QString dst{};
    dst.resize(src.size());

    QStringView::size_type sPos = 0; // src current pos
    QString::size_type dPos = 0; // dest current pos

    while (sPos < src.size()) {
        if (src[sPos] != '\\') {
            dst[dPos++] = src[sPos++];
        } else {
            if (++sPos >= src.size()) {
                // skip past the '\\'
                return QString(src.data(), src.size());
            }
            switch (src[sPos].unicode()) {
            case u'a':
                (dst)[dPos++] = '\a';
                break;
            case u'b':
                (dst)[dPos++] = '\b';
                break;
            case u'f':
                (dst)[dPos++] = '\f';
                break;
            case u'n':
                (dst)[dPos++] = '\n';
                break;
            case u'r':
                (dst)[dPos++] = '\r';
                break;
            case u't':
                (dst)[dPos++] = '\t';
                break;
            case u'v':
                (dst)[dPos++] = '\v';
                break;
            case u'\\':
                (dst)[dPos++] = '\\';
                break;
            case u'?':
                (dst)[dPos++] = '\?';
                break;
            case u'\'':
                (dst)[dPos++] = '\'';
                break;
            case u'"':
                (dst)[dPos++] = '\"';
                break;
            case u'0':
            case u'1':
            case u'2':
            case u'3':
            case u'4':
            case u'5':
            case u'6':
            case u'7': {
                // octal digit: 1 to 3 digits
                unsigned int ch = src[sPos].digitValue(); // digit 1
                if (sPos + 1 < src.size() && isOctalDigit(src[sPos + 1]))
                    ch = ch * 8 + src[++sPos].digitValue(); // digit 2
                if (sPos + 1 < src.size() && isOctalDigit(src[sPos + 1]))
                    ch = ch * 8 + src[++sPos].digitValue(); // digit 3
                if (ch > 0xff) {
                    return QString(src.data(), src.size());
                }
                (dst)[dPos++] = static_cast<char>(ch);
                break;
            }
            case u'x':
            case u'X': {
                if (sPos + 1 >= src.size() || !isHexDigit(src[sPos + 1])) {
                    return QString(src.data(), src.size());
                }
                unsigned int ch = 0;
                while (sPos + 1 < src.size() && isHexDigit(src[sPos + 1])) {
                    // Arbitrarily many hex digits
                    ch = (ch << 4) + hexDigitValue(src[++sPos]);
                }
                if (ch > 0xFF) {
                    return QString(src.data(), src.size());
                }
                dst[dPos++] = QChar(ch);
                break;
            }
            case u'u': {
                // \uhhhh => convert 4 hex digits to UTF-16
                char32_t rune = 0;
                if (sPos + 4 >= src.size()) {
                    return QString(src.data(), src.size());
                }
                for (int i = 0; i < 4; ++i) {
                    // Look one char ahead.
                    if (isHexDigit(src[sPos + 1])) {
                        rune = (rune << 4) + hexDigitValue(src[++sPos]);
                    } else {
                        return QString(src.data(), src.size());
                    }
                }

                if (QChar::isSurrogate(rune)) {
                    return QString(src.data(), src.size());
                }
                if (QChar::requiresSurrogates(rune)) {
                    dst[dPos++] = QChar::highSurrogate(rune);
                    dst[dPos++] = QChar::lowSurrogate(rune);
                } else {
                    dst[dPos++] = QChar(rune);
                }
                break;
            }
            case u'U': {
                // \Uhhhhhhhh => convert 8 hex digits to UTF-16
                char32_t rune = 0;
                if (sPos + 8 >= src.size()) {
                    return QString(src.data(), src.size());
                }
                for (int i = 0; i < 8; ++i) {
                    // Look one char ahead.
                    if (isHexDigit(src[sPos + 1])) {
                        // Don't change rune until we're sure this
                        // is within the Unicode limit, but do advance sPos.
                        uint32_t newrune = (rune << 4) + hexDigitValue(src[++sPos]);
                        if (newrune > 0x10FFFF) {
                            return QString(src.data(), src.size());
                        }
                        rune = newrune;
                    } else {
                        return QString(src.data(), src.size());
                    }
                }
                if (QChar::isSurrogate(rune)) {
                    return QString(src.data(), src.size());
                }
                if (QChar::requiresSurrogates(rune)) {
                    dst[dPos++] = QChar::highSurrogate(rune);
                    dst[dPos++] = QChar::lowSurrogate(rune);
                } else {
                    dst[dPos++] = QChar::highSurrogate(rune);
                }
                break;
            }
            default: {
                return QString(src.data(), src.size());
            }
            }
            sPos++; // Read past letter we escaped.
        }
    }
    dst.remove(dPos, dst.size() - dPos);
    return dst;
}