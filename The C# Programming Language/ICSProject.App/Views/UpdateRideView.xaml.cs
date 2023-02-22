using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using ICSProject.App.ViewModels;
using ICSProject.App.Wrappers;
using ICSProject.BL.Models;

namespace ICSProject.App.Views
{
    public partial class UpdateRideView : UserControlBase
    {
        public UpdateRideView()
        {
            InitializeComponent();
            this.SemiStopAdd.Visibility = Visibility.Hidden;
        }

        private void AppearSemiStopAdd(object sender, MouseButtonEventArgs e)
        {
            this.SemiStopAdd.Visibility = Visibility.Visible;
        }
        private void Delete_Semi(object sender, RoutedEventArgs e)
        {
            Button button = sender as Button;
            var car = button.DataContext as SemiStopWrapper;
            var vm = DataContext as UpdateRideViewModel;
            vm.SemiStopDeleted(car.Model);
        }
        private void DisappearSemiStopAdd(object sender, MouseButtonEventArgs e)
        {

            this.SemiStopAdd.Visibility = Visibility.Hidden;
        }
        private void CollapseSemi(object sender, RoutedEventArgs e)
        {
            this.SemiStopAdd.Visibility = Visibility.Hidden;
        }
    }
}
