import QtQuick

import SAK.Custom

SAKComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        ListElement { text: "CRC_8"; value: SAKCrcInterface.CRC_8}
        ListElement { text: "CRC_8_ITU"; value: SAKCrcInterface.CRC_8_ITU}
        ListElement { text: "CRC_8_ROHC"; value: SAKCrcInterface.CRC_8_ROHC}
        ListElement { text: "CRC_8_MAXIM"; value: SAKCrcInterface.CRC_8_MAXIM}

        ListElement { text: "CRC_16_IBM"; value: SAKCrcInterface.CRC_16_IBM}
        ListElement { text: "CRC_16_MAXIM"; value: SAKCrcInterface.CRC_16_MAXIM}
        ListElement { text: "CRC_16_USB"; value: SAKCrcInterface.CRC_16_USB}
        ListElement { text: "CRC_16_MODBUS"; value: SAKCrcInterface.CRC_16_MODBUS}
        ListElement { text: "CRC_16_CCITT"; value: SAKCrcInterface.CRC_16_CCITT}
        ListElement { text: "CRC_16_CCITT_FALSE"; value: SAKCrcInterface.CRC_16_CCITT_FALSE}
        ListElement { text: "CRC_16_x25"; value: SAKCrcInterface.CRC_16_x25}
        ListElement { text: "CRC_16_XMODEM"; value: SAKCrcInterface.CRC_16_XMODEM}
        ListElement { text: "CRC_16_DNP"; value: SAKCrcInterface.CRC_16_DNP}

        ListElement { text: "CRC_32"; value: SAKCrcInterface.CRC_32}
        ListElement { text: "CRC_32_MPEG2"; value: SAKCrcInterface.CRC_32_MPEG2}
    }
}
