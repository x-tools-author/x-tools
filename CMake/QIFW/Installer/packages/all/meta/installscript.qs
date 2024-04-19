/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

function Component()
{
    if (systemInfo.productType === "windows") {
        installer.execute("@TargetDir@/maintenancetool.exe", ["pr", "-c"]);

        installer.setValue("TargetDir", "@HomeDir@/AppData/Roaming/xTools/argConfigTargetDir");
        installer.setValue("StartMenuDir", "@HomeDir@/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/xTools/argConfigStartMenuDir");

        installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
        installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);

        gui.clickButton(buttons.NextButton);
    }
}

Component.prototype.createOperations = function()
{
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut",
                               "@TargetDir@/argProcessName.exe",
                               "@StartMenuDir@/argLinkName.lnk",
                               "workingDirectory=@TargetDir@",
                               "iconPath=@TargetDir@/icon.ico",
                               "description=Open Program");
        component.addOperation("CreateShortcut",
                               "@TargetDir@/argProcessName.exe",
                               "@DesktopDir@/argLinkName.lnk",
                               "workingDirectory=@TargetDir@",
                               "iconPath=@TargetDir@/icon.ico",
                               "description=Open Program");
        component.addOperation("CreateShortcut",
                               "@TargetDir@/maintenancetool.exe",
                               "@StartMenuDir@/Uninstall.lnk",
                               "workingDirectory=@TargetDir@",
                               "iconPath=%SystemRoot%/system32/SHELL32.dll",
                               "iconId=101",
                               "description=Uninstall Program",
                               "--start-uninstaller");
    }
}
