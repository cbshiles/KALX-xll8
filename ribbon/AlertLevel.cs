using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Office.Tools.Ribbon;
using Microsoft.Office.Core;
using Excel = Microsoft.Office.Interop.Excel;

namespace ribbon
{
    public partial class AlertLevel
    {
        private void AlertLevel_Load(object sender, RibbonUIEventArgs e)
        {
        }

        public void setAlertLevel(uint xal)
        {
            this.alertError.Checked = (xal & 1) != 0;
            this.alertWarning.Checked = (xal & 2) != 0;
            this.alertInfo.Checked = (xal & 4) != 0;
        }

        private void alertError_Click(object sender, RibbonControlEventArgs e)
        {
            Globals.ThisAddIn.Application.Run("XLL.ALERT.ERROR.TOGGLE");
        }

        private void alertWarning_Click(object sender, RibbonControlEventArgs e)
        {
            Globals.ThisAddIn.Application.Run("XLL.ALERT.WARNING.TOGGLE");
        }

        private void alertInfo_Click(object sender, RibbonControlEventArgs e)
        {
            Globals.ThisAddIn.Application.Run("XLL.ALERT.INFO.TOGGLE");
        }
    }
}
