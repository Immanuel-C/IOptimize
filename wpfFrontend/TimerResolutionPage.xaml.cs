using IOptimizeWPFFrontend;
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

namespace WPFFrontend
{
    /// <summary>
    /// Interaction logic for TimerResolutionPage.xaml
    /// </summary>
    public partial class TimerResolutionPage : Page
    {
        public TimerResolutionPage()
        {
            InitializeComponent();
        }

        private void SetMaxResolutionButton_Click(object sender, RoutedEventArgs e)
        {
            IOptimize.TimerResolutionValues values = IOptimize.IOptimizeQueryTimerResolution();
            IOptimize.IOptimizeSetTimerResolution(values.maxResolution);
            values = IOptimize.IOptimizeQueryTimerResolution();
            TimerResolutionLabel.Content = "Timer Resolution: " + ((float)values.currResolution / 10000.0f) + "ms";
        }

        private void SetMinResolutionButton_Click(object sender, RoutedEventArgs e)
        {
            IOptimize.TimerResolutionValues values = IOptimize.IOptimizeQueryTimerResolution();
            IOptimize.IOptimizeSetTimerResolution(values.minResolution);
            values = IOptimize.IOptimizeQueryTimerResolution();
            TimerResolutionLabel.Content = "Timer Resolution: " + ((float)values.currResolution / 10000.0f) + "ms";
        }
    }
}
