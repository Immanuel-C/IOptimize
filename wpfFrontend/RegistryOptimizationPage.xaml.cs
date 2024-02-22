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
    /// Interaction logic for RegistryOptimizationPage.xaml
    /// </summary>
    public partial class RegistryOptimizationPage : Page
    {
        public RegistryOptimizationPage()
        {
            InitializeComponent();

            GameBoxLabel.Visibility = Visibility.Hidden;
            GameComboBox.Visibility = Visibility.Hidden;

            OptimizeTypeComboBox.ItemsSource = optimizeTypeStrings; 
            GameComboBox.ItemsSource = gameOptimizeTypeStrings;
        }


        /// <summary>
        /// Interaction logic for RegistryOptimizationWindow.xaml
        /// </summary>
            string[] optimizeTypeStrings =
            {
                "FPS",
                "Input Latency"
            };
            string[] gameOptimizeTypeStrings =
            {
                "Fortnite",
                "Apex Legends",
                "None"
            };

            IOptimize.IOptimizeTypeFlags optimizeTypeFlags = new IOptimize.IOptimizeTypeFlags();
            IOptimize.IOptimizeTypeFlags gameOptimizeTypeFlags = new IOptimize.IOptimizeTypeFlags();



            private void OptimizeTypeComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                if (optimizeTypeFlags != IOptimize.IOptimizeTypeFlags.None)
                    optimizeTypeFlags = new IOptimize.IOptimizeTypeFlags();

                switch (OptimizeTypeComboBox.SelectedValue)
                {
                    case "FPS":
                        optimizeTypeFlags |= IOptimize.IOptimizeTypeFlags.FPS;
                        GameBoxLabel.Visibility = Visibility.Visible;
                        GameComboBox.Visibility = Visibility.Visible;
                        break;
                    case "Input Latency":
                        optimizeTypeFlags |= IOptimize.IOptimizeTypeFlags.Latency;
                        GameBoxLabel.Visibility = Visibility.Hidden;
                        GameComboBox.Visibility = Visibility.Hidden;
                        break;
                    default:
                        break;
                }

            }

            private void GameComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                if (gameOptimizeTypeFlags != IOptimize.IOptimizeTypeFlags.None)
                    gameOptimizeTypeFlags = new IOptimize.IOptimizeTypeFlags();

                switch (GameComboBox.SelectedValue)
                {
                    case "Fortnite":
                        gameOptimizeTypeFlags |= IOptimize.IOptimizeTypeFlags.Fortnite;
                        break;
                    case "Apex Legends":
                        gameOptimizeTypeFlags |= IOptimize.IOptimizeTypeFlags.ApexLegends;
                        break;
                    case "None":
                        gameOptimizeTypeFlags = IOptimize.IOptimizeTypeFlags.None;
                        break;
                    default:
                        break;
                }
            }

            private void ApplyOptimizationButton_Click(object sender, RoutedEventArgs e)
            {
                if (MsiModeCheckBox.IsChecked == true)
                {
                    IOptimize.IOptimizeSetGpuMsiMode(IOptimize.IOptimizeBool.True);
                }

                if (optimizeTypeFlags == IOptimize.IOptimizeTypeFlags.None)
                {
                    MessageBox.Show("You need to set an optimization type!", "Error!", MessageBoxButton.OK, MessageBoxImage.Error);
                }


                if (gameOptimizeTypeFlags == IOptimize.IOptimizeTypeFlags.None)
                {
                    IOptimize.IOptimizeSetRegistryTweaks(optimizeTypeFlags);
                    return;
                }

                IOptimize.IOptimizeSetRegistryTweaks(optimizeTypeFlags | gameOptimizeTypeFlags);

            }

            private void RevertOptimizationButton_Click(object sender, RoutedEventArgs e)
            {
                IOptimize.IOptimizeSetGpuMsiMode(IOptimize.IOptimizeBool.False);
                IOptimize.IOptimizeSetRegistryTweaks(IOptimize.IOptimizeTypeFlags.Revert);
            }



        }
    }
