using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using ICSProject.App.ViewModels;

namespace ICSProject.App.Views
{
    public partial class ProfileRideListView : UserControlBase
    {
        public ProfileRideListView()
        {
            InitializeComponent();
            DeleteButton.IsHitTestVisible = false;
            List.Visibility = Visibility.Hidden;
        }

        private void Button_visibility(object sender, RoutedEventArgs e)
        {
            var _ = (sender as ComboBox).SelectedItem as ComboBoxItem;
            if (_ == null) return;

            string? text = ((sender as ComboBox).SelectedItem as ComboBoxItem).Content as string;

            if (text == "Řidič")
            {
                DeleteButton.IsHitTestVisible = true;
                List.Visibility = Visibility.Visible;
            }
            else if(text == "Spolujezdec")
            {
                DeleteButton.IsHitTestVisible = false;
                List.Visibility = Visibility.Visible;
            }
        }
    }
}
