using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using Excel = Microsoft.Office.Interop.Excel;
using Office = Microsoft.Office.Core;
using Microsoft.Office.Tools.Excel;

//using System.Windows.Forms;


namespace ribbon
{
    public partial class ThisAddIn
    {
        private bool is64Bit()
        {
            return false; //!!! for now
        }
        // load all add-ins in current and subdirectories
        private void LoadAddins()
        {
            System.Reflection.Assembly assembly = System.Reflection.Assembly.GetExecutingAssembly();

            //Location is where the assembly is run from 
            string assemblyLocation = assembly.Location;

            //CodeBase is the location of the ClickOnce deployment files
            Uri codeBase = new Uri(assembly.CodeBase);
            string dir = Path.GetDirectoryName(codeBase.LocalPath.ToString());

            string cwd = Directory.GetCurrentDirectory();
            try
            {
 //               MessageBox.Show(dir);

                Directory.SetCurrentDirectory(dir);
                foreach (string xll in Directory.EnumerateFiles(".", "*.xll", System.IO.SearchOption.AllDirectories))
                {
                    this.Application.RegisterXLL(xll);
                }
            }
            finally 
            {
                Directory.SetCurrentDirectory(cwd);
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
