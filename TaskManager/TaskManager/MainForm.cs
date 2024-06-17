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

namespace TaskManager
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
            SetColumns();
            statusStrip1.Items.Add("");
            
        }

		private void timer1_Tick(object sender, EventArgs e)
		{
            LoadProcesses();
		}
        void SetColumns()
        {
            listViewProcesses.Columns.Add("PID");
            listViewProcesses.Columns.Add("Name");
        }
        void LoadProcesses()
        {
            listViewProcesses.Items.Clear();
            Process[] processes = Process.GetProcesses();
            for (int i = 0; i < processes.Length; i++) 
            {
                ListViewItem item = new ListViewItem();
                item.Text = processes[i].Id.ToString();
                item.SubItems.Add(processes[i].ProcessName);
                listViewProcesses.Items.Add(item);
            }
            statusStrip1.Items[0].Text = ($"Количество процессов: {listViewProcesses.Items.Count}");
        }
	}
}
