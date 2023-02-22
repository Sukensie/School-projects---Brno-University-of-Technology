using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ICSProject.App.ViewModels;

namespace ICSProject.App.Stores
{
    public class NavigationStore
    {
        public NavigationStore ()
        {

        }

        public event Action CurrentViewModelChanged;

        private void OnCurrentViewModelChanged()
        {
            CurrentViewModelChanged?.Invoke();
        }

        private ViewModelBase _currentViewModel;
        public ViewModelBase CurrentViewModel
        {
            get => _currentViewModel;
            set
            {
                _currentViewModel = value;
                OnCurrentViewModelChanged();
            }
        }

        public MainViewModel MainViewModel { get; set; }
        public RideViewModel RideViewModel { get; set; }
        public CreateUserViewModel CreateUserViewModel { get; set; }
        public ProfileViewModel ProfileViewModel { get; set; }
        public LoginViewModel LoginViewModel { get; set; }
        public GarageViewModel GarageViewModel { get; set; }
        public CreateRideViewModel CreateRideViewModel { get; set; }
        public RideDetailViewModel RideDetailViewModel { get; set; }
        public UpdateRideViewModel UpdateRideViewModel { get; set; }
    }
}
