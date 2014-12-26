namespace ribbon
{
    partial class AlertLevel : Microsoft.Office.Tools.Ribbon.RibbonBase
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        public AlertLevel()
            : base(Globals.Factory.GetRibbonFactory())
        {
            InitializeComponent();
        }

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.tab1 = this.Factory.CreateRibbonTab();
            this.group1 = this.Factory.CreateRibbonGroup();
            this.alertError = this.Factory.CreateRibbonCheckBox();
            this.alertWarning = this.Factory.CreateRibbonCheckBox();
            this.alertInfo = this.Factory.CreateRibbonCheckBox();
            this.tab1.SuspendLayout();
            this.group1.SuspendLayout();
            // 
            // tab1
            // 
            this.tab1.ControlId.ControlIdType = Microsoft.Office.Tools.Ribbon.RibbonControlIdType.Office;
            this.tab1.Groups.Add(this.group1);
            this.tab1.Label = "TabAddIns";
            this.tab1.Name = "tab1";
            // 
            // group1
            // 
            this.group1.Items.Add(this.alertError);
            this.group1.Items.Add(this.alertWarning);
            this.group1.Items.Add(this.alertInfo);
            this.group1.Label = "Alert Level";
            this.group1.Name = "group1";
            // 
            // alertError
            // 
            this.alertError.Label = "Error";
            this.alertError.Name = "alertError";
            this.alertError.Click += new Microsoft.Office.Tools.Ribbon.RibbonControlEventHandler(this.alertError_Click);
            // 
            // alertWarning
            // 
            this.alertWarning.Label = "Warning";
            this.alertWarning.Name = "alertWarning";
            this.alertWarning.Click += new Microsoft.Office.Tools.Ribbon.RibbonControlEventHandler(this.alertWarning_Click);
            // 
            // alertInfo
            // 
            this.alertInfo.Label = "Information";
            this.alertInfo.Name = "alertInfo";
            this.alertInfo.Click += new Microsoft.Office.Tools.Ribbon.RibbonControlEventHandler(this.alertInfo_Click);
            // 
            // AlertLevel
            // 
            this.Name = "AlertLevel";
            this.RibbonType = "Microsoft.Excel.Workbook";
            this.Tabs.Add(this.tab1);
            this.Load += new Microsoft.Office.Tools.Ribbon.RibbonUIEventHandler(this.AlertLevel_Load);
            this.tab1.ResumeLayout(false);
            this.tab1.PerformLayout();
            this.group1.ResumeLayout(false);
            this.group1.PerformLayout();

        }

        #endregion

        internal Microsoft.Office.Tools.Ribbon.RibbonTab tab1;
        internal Microsoft.Office.Tools.Ribbon.RibbonGroup group1;
        internal Microsoft.Office.Tools.Ribbon.RibbonCheckBox alertError;
        internal Microsoft.Office.Tools.Ribbon.RibbonCheckBox alertWarning;
        internal Microsoft.Office.Tools.Ribbon.RibbonCheckBox alertInfo;
    }

    partial class ThisRibbonCollection
    {
        internal AlertLevel AlertLevel
        {
            get { return this.GetRibbon<AlertLevel>(); }
        }
    }
}
