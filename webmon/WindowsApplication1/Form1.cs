using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace WindowsApplication1
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txtSearch;
		private System.Windows.Forms.Button btnSearch;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label lblSearchSummary;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.ListBox lstResults;
		private System.Data.DataSet dsSeen;
		private System.Windows.Forms.Button btnClear;
		private System.Windows.Forms.Button btnLoad;
		private System.Windows.Forms.Button btnSave;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.TextBox txtExcludeAdd;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.ListBox lstExcluded;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button3;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.txtSearch = new System.Windows.Forms.TextBox();
			this.btnSearch = new System.Windows.Forms.Button();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.lblSearchSummary = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.btnSave = new System.Windows.Forms.Button();
			this.btnLoad = new System.Windows.Forms.Button();
			this.btnClear = new System.Windows.Forms.Button();
			this.lstResults = new System.Windows.Forms.ListBox();
			this.dsSeen = new System.Data.DataSet();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.txtExcludeAdd = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.lstExcluded = new System.Windows.Forms.ListBox();
			this.button2 = new System.Windows.Forms.Button();
			this.button3 = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dsSeen)).BeginInit();
			this.groupBox4.SuspendLayout();
			this.SuspendLayout();
			// 
			// txtSearch
			// 
			this.txtSearch.Location = new System.Drawing.Point(8, 16);
			this.txtSearch.Multiline = true;
			this.txtSearch.Name = "txtSearch";
			this.txtSearch.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.txtSearch.Size = new System.Drawing.Size(392, 72);
			this.txtSearch.TabIndex = 0;
			this.txtSearch.Text = "";
			this.txtSearch.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.txtSearch_KeyPress);
			// 
			// btnSearch
			// 
			this.btnSearch.Location = new System.Drawing.Point(408, 16);
			this.btnSearch.Name = "btnSearch";
			this.btnSearch.Size = new System.Drawing.Size(64, 24);
			this.btnSearch.TabIndex = 1;
			this.btnSearch.Text = "Search";
			this.btnSearch.Click += new System.EventHandler(this.btnSearch_Click);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.button3);
			this.groupBox1.Controls.Add(this.lblSearchSummary);
			this.groupBox1.Location = new System.Drawing.Point(8, 336);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(480, 80);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Search results summary";
			// 
			// lblSearchSummary
			// 
			this.lblSearchSummary.Location = new System.Drawing.Point(8, 16);
			this.lblSearchSummary.Name = "lblSearchSummary";
			this.lblSearchSummary.Size = new System.Drawing.Size(464, 32);
			this.lblSearchSummary.TabIndex = 0;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.txtSearch);
			this.groupBox2.Controls.Add(this.btnSearch);
			this.groupBox2.Location = new System.Drawing.Point(8, 8);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(480, 96);
			this.groupBox2.TabIndex = 4;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Search";
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.btnSave);
			this.groupBox3.Controls.Add(this.btnLoad);
			this.groupBox3.Controls.Add(this.btnClear);
			this.groupBox3.Controls.Add(this.lstResults);
			this.groupBox3.Location = new System.Drawing.Point(496, 8);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(480, 408);
			this.groupBox3.TabIndex = 5;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "Results";
			// 
			// btnSave
			// 
			this.btnSave.Location = new System.Drawing.Point(200, 376);
			this.btnSave.Name = "btnSave";
			this.btnSave.TabIndex = 3;
			this.btnSave.Text = "Save XML";
			this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
			// 
			// btnLoad
			// 
			this.btnLoad.Location = new System.Drawing.Point(120, 376);
			this.btnLoad.Name = "btnLoad";
			this.btnLoad.TabIndex = 2;
			this.btnLoad.Text = "Load XML";
			this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
			// 
			// btnClear
			// 
			this.btnClear.Location = new System.Drawing.Point(8, 376);
			this.btnClear.Name = "btnClear";
			this.btnClear.TabIndex = 1;
			this.btnClear.Text = "Clear";
			this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
			// 
			// lstResults
			// 
			this.lstResults.Location = new System.Drawing.Point(8, 16);
			this.lstResults.Name = "lstResults";
			this.lstResults.Size = new System.Drawing.Size(464, 355);
			this.lstResults.TabIndex = 0;
			this.lstResults.DoubleClick += new System.EventHandler(this.lstResults_DoubleClick);
			// 
			// dsSeen
			// 
			this.dsSeen.DataSetName = "dsSeen";
			this.dsSeen.Locale = new System.Globalization.CultureInfo("en-US");
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.button2);
			this.groupBox4.Controls.Add(this.lstExcluded);
			this.groupBox4.Controls.Add(this.button1);
			this.groupBox4.Controls.Add(this.txtExcludeAdd);
			this.groupBox4.Location = new System.Drawing.Point(8, 112);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(480, 208);
			this.groupBox4.TabIndex = 6;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "Exclude";
			// 
			// txtExcludeAdd
			// 
			this.txtExcludeAdd.Location = new System.Drawing.Point(8, 16);
			this.txtExcludeAdd.Name = "txtExcludeAdd";
			this.txtExcludeAdd.Size = new System.Drawing.Size(392, 20);
			this.txtExcludeAdd.TabIndex = 0;
			this.txtExcludeAdd.Text = "";
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(408, 16);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(64, 24);
			this.button1.TabIndex = 1;
			this.button1.Text = "Add";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// lstExcluded
			// 
			this.lstExcluded.Location = new System.Drawing.Point(8, 40);
			this.lstExcluded.Name = "lstExcluded";
			this.lstExcluded.Size = new System.Drawing.Size(392, 160);
			this.lstExcluded.TabIndex = 2;
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(408, 48);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(64, 23);
			this.button2.TabIndex = 3;
			this.button2.Text = "Remove";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// button3
			// 
			this.button3.Location = new System.Drawing.Point(408, 48);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(64, 24);
			this.button3.TabIndex = 1;
			this.button3.Text = "View";
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(984, 421);
			this.Controls.Add(this.groupBox4);
			this.Controls.Add(this.groupBox3);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Name = "Form1";
			this.Text = "TOWER Web Monitor";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox2.ResumeLayout(false);
			this.groupBox3.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dsSeen)).EndInit();
			this.groupBox4.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}

		private void btnSearch_Click(object sender, System.EventArgs e)
		{
			try
			{
				Google.GoogleSearchService gs = new Google.GoogleSearchService();
				String search = txtSearch.Text;

				// Exclude listed sites
				foreach(string site in lstExcluded.Items)
				{
					search += " -site:";
					search += site;
				}

				// Now search
				lblSearchSummary.Text = "Searching...";
				bool searching = true;
				int inset = 0;
				int pass = 0;
				while(searching)
				{
					pass++;
					// This key is tied to michael.still@towersoft.com.au -- please don't misuse it
					Google.GoogleSearchResult res = gs.doGoogleSearch("pIl0UvRQFHKPGJHnQ8MttiMXESmKQdT9", search, 
						inset, 10, false, "", false, "", "", "");
					inset += 10;

					for(int i = 0; i < res.resultElements.Length; i++)
					{
						String select = "URL = '" + res.resultElements[i].URL + "'";
						System.Data.DataRow[] rows = dsSeen.Tables["URLs"].Select(select);

						if(rows.Length == 0)
						{
							// Not good style!
							String title = res.resultElements[i].title;
							String cleanTitle = "";
							bool inText = true;

							for(int j = 0; j < title.Length; j++)
							{
								if(title[j] == '<')
									inText = false;

								if(inText)
									cleanTitle += title[j];

								if(title[j] == '>')
									inText = true;
							}

							// Do something with the row
							lstResults.Items.Add(cleanTitle + " (" + res.resultElements[i].URL + ")");
							System.Data.DataRow srow = dsSeen.Tables["URLs"].NewRow();
							srow["URL"] = res.resultElements[i].URL;
							dsSeen.Tables["URLs"].Rows.Add(srow);
						}
					}

					lblSearchSummary.Text = "Estimated of hits: " + res.estimatedTotalResultsCount + 
						" Pass: " + pass + " Extracted hits: " + lstResults.Items.Count;

					if(res.endIndex < inset)
						searching = false;
				}

				lblSearchSummary.Text = "Search finished. " + lstResults.Items.Count + " items found";
			}
		catch(System.Exception excep)
			{
				lblSearchSummary.Text = "Caught an exception searching: " + excep.Message;
			}
		}

		private void txtSearch_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			if(e.KeyChar == 13)
			{
				btnSearch_Click(sender, e);
			}
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
			dsSeen.Tables.Add("URLs");
			dsSeen.Tables["URLs"].Columns.Add("URL");

			dsSeen.Tables.Add("Search");
			dsSeen.Tables["Search"].Columns.Add("Term");

			// This entry is only used by an old version of the schema
			dsSeen.Tables["Search"].Columns.Add("Exclude");

			dsSeen.Tables.Add("Excluded");
			dsSeen.Tables["Excluded"].Columns.Add("Exclude");
		}

		private void btnClear_Click(object sender, System.EventArgs e)
		{
			lstResults.Items.Clear();
		}

		private void btnSave_Click(object sender, System.EventArgs e)
		{
			SaveFileDialog saveas = new SaveFileDialog();

			saveas.DefaultExt = "xml";
			if(saveas.ShowDialog() == DialogResult.OK)
			{
				dsSeen.Tables["Search"].Clear();
				System.Data.DataRow row = dsSeen.Tables["Search"].NewRow();
				row["Term"] = txtSearch.Text;
				dsSeen.Tables["Search"].Rows.Add(row);
				
				// Check if we're excluding
				foreach(string site in lstExcluded.Items)
				{
					System.Data.DataRow siterow = dsSeen.Tables["Excluded"].NewRow();
					siterow["Exclude"] = site;
					dsSeen.Tables["Excluded"].Rows.Add(siterow);
				}

				// And then write
				dsSeen.WriteXml(saveas.FileName);
			}
		}

		private void btnLoad_Click(object sender, System.EventArgs e)
		{
			OpenFileDialog open = new OpenFileDialog();

			open.DefaultExt = "xml";
			if(open.ShowDialog() == DialogResult.OK)
			{
				dsSeen.Clear();
				dsSeen.ReadXml(open.FileName);
				
				System.Data.DataRow[] rows = dsSeen.Tables["Search"].Select("");
				if(rows.Length > 0)
				{
					txtSearch.Text = rows[0]["Term"].ToString();
					
					// Old XML schema
					if((rows[0]["Exclude"].ToString() == "yes"))
					{
						lstExcluded.Items.Add("towersoft.com.au");
						lstExcluded.Items.Add("towersoft.com");
						lstExcluded.Items.Add("towersoft.co.uk");
						lstExcluded.Items.Add("ustrim.com");
						lstExcluded.Items.Add("trimcontext.com");
					}
				}

				System.Data.DataRow[] siterows = dsSeen.Tables["Excluded"].Select("");
				foreach(System.Data.DataRow siterow in siterows)
				{
					lstExcluded.Items.Add(siterow["Exclude"]);
				}
			}
		}

		private void lstResults_DoubleClick(object sender, System.EventArgs e)
		{
			Int32 idx = lstResults.SelectedIndex;
			if(idx != -1)
			{
				System.Diagnostics.Process proc;
				String text = lstResults.Items[idx].ToString();
				String URL = "";
				bool inUrl = false;

				// TODO: this is broken
				for(int j = 0; j < text.Length; j++)
				{
					if(text[j] == ')')
						inUrl = false;

					if(inUrl)
						URL += text[j];

					if(text[j] == '(')
						inUrl = true;
				}

				proc = System.Diagnostics.Process.Start(
					'"' + "c:/program files/internet explorer/iexplore.exe" + '"', 
					URL);
				//proc.WaitForExit();
			}
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			lstExcluded.Items.Add(txtExcludeAdd.Text);
			txtExcludeAdd.Text = "";
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			lstExcluded.Items.RemoveAt(lstExcluded.SelectedIndex);
		}

		private void button3_Click(object sender, System.EventArgs e)
		{
			System.Windows.Forms.MessageBox.Show(lblSearchSummary.Text);
		}
	}
}
