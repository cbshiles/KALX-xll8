using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Excel = Microsoft.Office.Interop.Excel;
using Office = Microsoft.Office.Core;
using Microsoft.Office.Tools.Excel;

namespace ribbon
{
    public partial class ThisAddIn
    {
        // load all add-ins in current and subdirectories
        private void LoadAddins()
        {
//            string dir = Directory.GetCurrentDirectory();
            Directory.SetCurrentDirectory(@"C:\Users\Keith\Source\Repos\xll8\Debug\");
            foreach (string xll in Directory.EnumerateFiles(".", "*.xll", System.IO.SearchOption.AllDirectories))
            {
                this.Application.RegisterXLL(xll);
                Microsoft.Office.Tools.Ribbon.RibbonCheckBox cb = Globals.Ribbons.GetRibbon<AlertLevel>().Factory.CreateRibbonCheckBox();
                // !!! read only
//                Globals.Ribbons.GetRibbon<AlertLevel>().groupModule.Items.Add(cb);
//                cb.Label = xll;
//                cb.Name = "module" + xll;


            }
        }
        private void ThisAddIn_Startup(object sender, System.EventArgs e)
        {
            LoadAddins();

            uint xal = (uint)Globals.ThisAddIn.Application.Run("XLL.ALERT.LEVEL", Type.Missing);
            Globals.Ribbons.GetRibbon<AlertLevel>().setAlertLevel(xal);
        }

        private void ThisAddIn_Shutdown(object sender, System.EventArgs e)
        {
        }


        #region VSTO generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InternalStartup()
        {
            this.Startup += new System.EventHandler(ThisAddIn_Startup);
            this.Shutdown += new System.EventHandler(ThisAddIn_Shutdown);
        }
        
        #endregion
    }
}
