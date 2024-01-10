using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using IOptimizeWPFFrontend;
using WPFFrontend;

namespace IOptimizeWPFFrontend
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            
            InitializeComponent();
        }

        private void SwitchToRegistryOptimizationPage_Click(object sender, RoutedEventArgs e)
        {
            RegistryOptimizationWindow window = new RegistryOptimizationWindow();

            window.Show();
        }
    }

    
}
