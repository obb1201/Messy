namespace BuildTool
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.FBXProcessorButton = new System.Windows.Forms.Button();
            this.FBXButton = new System.Windows.Forms.Button();
            this.FBXProcessorText = new System.Windows.Forms.TextBox();
            this.FBXText = new System.Windows.Forms.TextBox();
            this.SaveAsText = new System.Windows.Forms.TextBox();
            this.SaveAsButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(511, 317);
            this.button1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(127, 61);
            this.button1.TabIndex = 0;
            this.button1.Text = "FBXProcessor";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // FBXProcessorButton
            // 
            this.FBXProcessorButton.Location = new System.Drawing.Point(32, 38);
            this.FBXProcessorButton.Name = "FBXProcessorButton";
            this.FBXProcessorButton.Size = new System.Drawing.Size(159, 23);
            this.FBXProcessorButton.TabIndex = 2;
            this.FBXProcessorButton.Text = "FBXProcessor";
            this.FBXProcessorButton.UseVisualStyleBackColor = true;
            this.FBXProcessorButton.Click += new System.EventHandler(this.FBXProcessorButton_Click);
            // 
            // FBXButton
            // 
            this.FBXButton.Location = new System.Drawing.Point(32, 220);
            this.FBXButton.Name = "FBXButton";
            this.FBXButton.Size = new System.Drawing.Size(159, 23);
            this.FBXButton.TabIndex = 3;
            this.FBXButton.Text = "Select FBX";
            this.FBXButton.UseVisualStyleBackColor = true;
            this.FBXButton.Click += new System.EventHandler(this.FBXButton_Click);
            // 
            // FBXProcessorText
            // 
            this.FBXProcessorText.Location = new System.Drawing.Point(32, 68);
            this.FBXProcessorText.Name = "FBXProcessorText";
            this.FBXProcessorText.Size = new System.Drawing.Size(410, 21);
            this.FBXProcessorText.TabIndex = 4;
            // 
            // FBXText
            // 
            this.FBXText.Location = new System.Drawing.Point(32, 249);
            this.FBXText.Name = "FBXText";
            this.FBXText.Size = new System.Drawing.Size(410, 21);
            this.FBXText.TabIndex = 5;
            // 
            // SaveAsText
            // 
            this.SaveAsText.Location = new System.Drawing.Point(32, 163);
            this.SaveAsText.Name = "SaveAsText";
            this.SaveAsText.Size = new System.Drawing.Size(410, 21);
            this.SaveAsText.TabIndex = 7;
            // 
            // SaveAsButton
            // 
            this.SaveAsButton.Location = new System.Drawing.Point(32, 134);
            this.SaveAsButton.Name = "SaveAsButton";
            this.SaveAsButton.Size = new System.Drawing.Size(159, 23);
            this.SaveAsButton.TabIndex = 6;
            this.SaveAsButton.Text = "Save .model As...";
            this.SaveAsButton.UseVisualStyleBackColor = true;
            this.SaveAsButton.Click += new System.EventHandler(this.SaveAsButton_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(684, 442);
            this.Controls.Add(this.SaveAsText);
            this.Controls.Add(this.SaveAsButton);
            this.Controls.Add(this.FBXText);
            this.Controls.Add(this.FBXProcessorText);
            this.Controls.Add(this.FBXButton);
            this.Controls.Add(this.FBXProcessorButton);
            this.Controls.Add(this.button1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button FBXProcessorButton;
        private System.Windows.Forms.Button FBXButton;
        private System.Windows.Forms.TextBox FBXProcessorText;
        private System.Windows.Forms.TextBox FBXText;
        private System.Windows.Forms.TextBox SaveAsText;
        private System.Windows.Forms.Button SaveAsButton;
    }
}

