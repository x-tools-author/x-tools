import QtQuick

import SAK.Custom

EDComboBox {
    id: root
    textRole: "text"
    valueRole: "value"
    model: ListModel {
        ListElement { text: "CRC_8"; value: EDCrc.CRC_8}
        ListElement { text: "CRC_8_ITU"; value: EDCrc.CRC_8_ITU}
        ListElement { text: "CRC_8_ROHC"; value: EDCrc.CRC_8_ROHC}
        ListElement { text: "CRC_8_MAXIM"; value: EDCrc.CRC_8_MAXIM}

        ListElement { text: "CRC_16_IBM"; value: EDCrc.CRC_16_IBM}
        ListElement { text: "CRC_16_MAXIM"; value: EDCrc.CRC_16_MAXIM}
        ListElement { text: "CRC_16_USB"; value: EDCrc.CRC_16_USB}
        ListElement { text: "CRC_16_MODBUS"; value: EDCrc.CRC_16_MODBUS}
        ListElement { text: "CRC_16_CCITT"; value: EDCrc.CRC_16_CCITT}
        ListElement { text: "CRC_16_CCITT_FALSE"; value: EDCrc.CRC_16_CCITT_FALSE}
        ListElement { text: "CRC_16_x25"; value: EDCrc.CRC_16_x25}
        ListElement { text: "CRC_16_XMODEM"; value: EDCrc.CRC_16_XMODEM}
        ListElement { text: "CRC_16_DNP"; value: EDCrc.CRC_16_DNP}

        ListElement { text: "CRC_32"; value: EDCrc.CRC_32}
        ListElement { text: "CRC_32_MPEG2"; value: EDCrc.CRC_32_MPEG2}
    }
}
