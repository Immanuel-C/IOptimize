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

            OptimizeTypeComboBox.ItemsSource = optimizeTypeStrings; ;
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

            IOptimizeFunctions.IOptimizeTypeFlags optimizeTypeFlags = new IOptimizeFunctions.IOptimizeTypeFlags();
            IOptimizeFunctions.IOptimizeTypeFlags gameOptimizeTypeFlags = new IOptimizeFunctions.IOptimizeTypeFlags();



            private void OptimizeTypeComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                if (optimizeTypeFlags != IOptimizeFunctions.IOptimizeTypeFlags.None)
                    optimizeTypeFlags = new IOptimizeFunctions.IOptimizeTypeFlags();

                switch (OptimizeTypeComboBox.SelectedValue)
                {
                    case "FPS":
                        optimizeTypeFlags |= IOptimizeFunctions.IOptimizeTypeFlags.FPS;
                        GameBoxLabel.Visibility = Visibility.Visible;
                        GameComboBox.Visibility = Visibility.Visible;
                        break;
                    case "Input Latency":
                        optimizeTypeFlags |= IOptimizeFunctions.IOptimizeTypeFlags.Latency;
                        GameBoxLabel.Visibility = Visibility.Hidden;
                        GameComboBox.Visibility = Visibility.Hidden;
                        break;
                    default:
                        break;
                }

            }

            private void GameComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
            {
                if (gameOptimizeTypeFlags != IOptimizeFunctions.IOptimizeTypeFlags.None)
                    gameOptimizeTypeFlags = new IOptimizeFunctions.IOptimizeTypeFlags();

                switch (GameComboBox.SelectedValue)
                {
                    case "Fortnite":
                        gameOptimizeTypeFlags |= IOptimizeFunctions.IOptimizeTypeFlags.Fortnite;
                        break;
                    case "Apex Legends":
                        gameOptimizeTypeFlags |= IOptimizeFunctions.IOptimizeTypeFlags.ApexLegends;
                        break;
                    case "None":
                        gameOptimizeTypeFlags = IOptimizeFunctions.IOptimizeTypeFlags.None;
                        break;
                    default:
                        break;
                }
            }

            private void ApplyOptimizationButton_Click(object sender, RoutedEventArgs e)
            {
                if (MsiModeCheckBox.IsChecked == true)
                {
                    IOptimizeFunctions.IOptimizeSetGpuMsiMode(1);
                }

                if (optimizeTypeFlags == IOptimizeFunctions.IOptimizeTypeFlags.None)
                {
                    MessageBox.Show("You need to set an optimization type!", "Error!", MessageBoxButton.OK, MessageBoxImage.Error);
                }


                if (gameOptimizeTypeFlags == IOptimizeFunctions.IOptimizeTypeFlags.None)
                {
                    IOptimizeFunctions.IOptimizeSetRegistryTweaks(optimizeTypeFlags);
                    return;
                }

                IOptimizeFunctions.IOptimizeSetRegistryTweaks(optimizeTypeFlags | gameOptimizeTypeFlags);

            }

            private void RevertOptimizationButton_Click(object sender, RoutedEventArgs e)
            {
                IOptimizeFunctions.IOptimizeSetGpuMsiMode(0);
                IOptimizeFunctions.IOptimizeSetRegistryTweaks(IOptimizeFunctions.IOptimizeTypeFlags.Revert);
            }



        }
    }
