using System.Windows;
using ICSProject.App.ViewModels;

namespace ICSProject.App.Views
{
    public partial class MainWindow : Window
    {
        //public MainWindow(MainViewModel mainViewModel)
        //{
        //    InitializeComponent();
        //    DataContext = mainViewModel;
        //}

        public MainWindow(MainViewModel mainViewModel)
        {
            InitializeComponent();
            DataContext = mainViewModel;
        }
    }

}