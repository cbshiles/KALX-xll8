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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(AlertLevel));
            this.tab1 = this.Factory.CreateRibbonTab();
            this.alert = this.Factory.CreateRibbonGroup();
            this.alertError = this.Factory.CreateRibbonCheckBox();
            this.alertWarning = this.Factory.CreateRibbonCheckBox();
            this.alertInfo = this.Factory.CreateRibbonCheckBox();
            this.monte = this.Factory.CreateRibbonGroup();
            this.button1 = this.Factory.CreateRibbonButton();
            this.button2 = this.Factory.CreateRibbonButton();
            this.button3 = this.Factory.CreateRibbonButton();
            this.tab1.SuspendLayout();
            this.alert.SuspendLayout();
            this.monte.SuspendLayout();
            // 
            // tab1
            // 
            this.tab1.ControlId.ControlIdType = Microsoft.Office.Tools.Ribbon.RibbonControlIdType.Office;
            this.tab1.Groups.Add(this.alert);
            this.tab1.Groups.Add(this.monte);
            this.tab1.Label = "TabAddIns";
            this.tab1.Name = "tab1";
            // 
            // alert
            // 
            this.alert.Items.Add(this.alertError);
            this.alert.Items.Add(this.alertWarning);
            this.alert.Items.Add(this.alertInfo);
            this.alert.Label = "Alert Level";
            this.alert.Name = "alert";
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
            // monte
            // 
            this.monte.Items.Add(this.button1);
            this.monte.Items.Add(this.button2);
            this.monte.Items.Add(this.button3);
            this.monte.Label = "Monte";
            this.monte.Name = "monte";
            // 
            // button1
            // 
            this.button1.Image = ((System.Drawing.Image)(resources.GetObject("button1.Image")));
            this.button1.Label = "Run";
            this.button1.Name = "button1";
            this.button1.ShowImage = true;
            // 
            // button2
            // 
            this.button2.Image = ((System.Drawing.Image)(resources.GetObject("button2.Image")));
            this.button2.Label = "Step";
            this.button2.Name = "button2";
            this.button2.ShowImage = true;
            // 
            // button3
            // 
            this.button3.Image = ((System.Drawing.Image)(resources.GetObject("button3.Image")));
            this.button3.Label = "Reset";
            this.button3.Name = "button3";
            this.button3.ShowImage = true;
            // 
            // AlertLevel
            // 
            this.Name = "AlertLevel";
            this.RibbonType = "Microsoft.Excel.Workbook";
            this.Tabs.Add(this.tab1);
            this.Load += new Microsoft.Office.Tools.Ribbon.RibbonUIEventHandler(this.AlertLevel_Load);
            this.tab1.ResumeLayout(false);
            this.tab1.PerformLayout();
            this.alert.ResumeLayout(false);
            this.alert.PerformLayout();
            this.monte.ResumeLayout(false);
            this.monte.PerformLayout();

        }

        #endregion

        internal Microsoft.Office.Tools.Ribbon.RibbonTab tab1;
        internal Microsoft.Office.Tools.Ribbon.RibbonGroup alert;
        internal Microsoft.Office.Tools.Ribbon.RibbonCheckBox alertError;
        internal Microsoft.Office.Tools.Ribbon.RibbonCheckBox alertWarning;
        internal Microsoft.Office.Tools.Ribbon.RibbonCheckBox alertInfo;
        internal Microsoft.Office.Tools.Ribbon.RibbonGroup monte;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button1;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button2;
        internal Microsoft.Office.Tools.Ribbon.RibbonButton button3;
    }

    partial class ThisRibbonCollection
    {
        internal AlertLevel AlertLevel
        {
            get { return this.GetRibbon<AlertLevel>(); }
        }
    }
}
