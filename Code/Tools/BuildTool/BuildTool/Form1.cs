using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;


namespace BuildTool
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Process p = new Process();
            p.StartInfo.FileName = FBXProcessorText.Text;
            p.StartInfo.Arguments = FBXText.Text;
            p.Start();
        }

        private void FBXProcessorButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "EXE FILE(*.exe)|*.exe|ALL (*.*)|*.*";
            ofd.Multiselect = true;
            DialogResult dr = ofd.ShowDialog();

            if (dr == DialogResult.OK)
            {
                string fileFullName = ofd.FileName;

                FBXProcessorText.Text = fileFullName;
            }
        }

        private void FBXButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "FBX FILE(*.fbx)|*.fbx|ALL(*.*)|*.*";
            ofd.Multiselect = true;
            DialogResult dr = ofd.ShowDialog();

            if (dr == DialogResult.OK)
            {
                string fileName = ofd.SafeFileName;
                string fileFullName = ofd.FileName;

                //FBXText.Text = fileFullName + " \\test\\box.model";
                FBXText.Text = fileFullName + " " + SaveAsText.Text + "\\" + fileName.Replace(".fbx",".model");
            }
        }

        private void SaveAsButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();

            DialogResult dr = fbd.ShowDialog();

            if (dr == DialogResult.OK)
            {
                string folderpath = fbd.SelectedPath;

                SaveAsText.Text = folderpath;
            }
        }
    }
}