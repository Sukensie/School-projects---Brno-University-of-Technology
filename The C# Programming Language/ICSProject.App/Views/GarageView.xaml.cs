using System.Windows;
using ICSProject.App.ViewModels;

namespace ICSProject.App.Views
{
    public partial class GarageView : UserControlBase
    {
        public GarageView()
        {
            InitializeComponent();
        }

        private void ShowAdd(object sender, RoutedEventArgs e)
        {
            this.EditView.Visibility = Visibility.Hidden;
            this.CreateView.Visibility = Visibility.Visible;
        }

        private void ShowEdit(object sender, RoutedEventArgs e)
        {
            this.CreateView.Visibility = Visibility.Hidden;
            this.EditView.Visibility = Visibility.Visible;
        }
    }
}
