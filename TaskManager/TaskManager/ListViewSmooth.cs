using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TaskManager
{
	internal class ListViewSmooth:ListView
	{
		public ListViewSmooth() 
		{
			SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.AllPaintingInWmPaint, true);
		}
	}
}
