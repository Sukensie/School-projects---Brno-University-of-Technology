using ICSProject.App.ViewModels;
using ICSProject.App.Wrappers;
using ICSProject.BL.Models;
using System.Windows;
using System.Windows.Controls;

namespace ICSProject.App.Views
{
    public partial class CarsListView : UserControlBase
    {
        public CarsListView()
        {
            InitializeComponent();
        }
        private void Remove_Car(object sender, RoutedEventArgs e)
        {
            Button button = sender as Button;
            var car = button.DataContext as CarListModel;
            var vm = DataContext as CarsListViewModel;
            vm.DeleteAsync(car);
        }
    }
}
