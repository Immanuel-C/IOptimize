using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
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
        RegistryOptimizationPage registryOptimizatonPage = new RegistryOptimizationPage();
        ResolutionSwitcherPage resolutionSwitcherPage = new ResolutionSwitcherPage();
        TimerResolutionPage timerResolutionPage = new TimerResolutionPage();
        HomePage homePage = new HomePage();

        public MainWindow()
        {
            InitializeComponent();
            WindowFrame.Content = homePage;
        }

        private void SwitchToRegistryOptimizationPage_Click(object sender, RoutedEventArgs e)
        {
            WindowFrame.Content = registryOptimizatonPage;
            Title = "IOptimize - Registry Optimizations";
        }

        private void SwitchToResolutionChangerWindow_Click(object sender, RoutedEventArgs e)
        {
            WindowFrame.Content = resolutionSwitcherPage;
            Title = "IOptimize - Resolution Switcher";
        }

        private void SwitchToTimerResolutionPage_Click(object sender, RoutedEventArgs e)
        {
            WindowFrame.Content = timerResolutionPage;
            Title = "IOptimize - Timer Resolution";
        }

        private void SwitchToHomePage_Click(object sender, RoutedEventArgs e)
        {
            WindowFrame.Content = homePage;
            Title = "IOptimize";
        }

    }

    
}