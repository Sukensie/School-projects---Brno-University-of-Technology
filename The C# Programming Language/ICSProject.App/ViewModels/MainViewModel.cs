using System;
using System.ComponentModel;
using System.Globalization;
using System.Runtime.CompilerServices;
using System.Threading.Tasks;
using System.Windows.Input;
using ICSProject.App.Services;
using ICSProject.App.Commands;
using ICSProject.App.Factories;
using ICSProject.App.Messages;
using ICSProject.App.Stores;
using ICSProject.App.Views;
using ICSProject.App.Wrappers;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;


namespace ICSProject.App.ViewModels
{
    public class MainViewModel : ViewModelBase, IMainViewModel
    {
        public MainViewModel(
            LoginViewModel loginViewModel,
            RideViewModel rideViewModel,
            GarageViewModel garageViewModel,
            CreateUserViewModel createUserViewModel,
            CreateRideViewModel createRideViewModel,
            ProfileViewModel profileViewModel,
            MenuViewModel menuViewModel,
            NavigationStore navigationStore,
            UserFacade userFacade,
            RideDetailViewModel rideDetailViewModel,
            UpdateRideViewModel updateRideViewModel,
            IMediator mediator
        )
        {
            RideViewModel = rideViewModel;
            RideDetailViewModel = rideDetailViewModel;
            CreateUserViewModel = createUserViewModel;
            ProfileViewModel = profileViewModel;
            MenuViewModel = menuViewModel;
            UpdateRideViewModel = updateRideViewModel;
            _userFacade = userFacade;

            _navigationStore = navigationStore;
            _navigationStore.RideViewModel = rideViewModel;
            _navigationStore.LoginViewModel = loginViewModel;
            _navigationStore.GarageViewModel = garageViewModel;
            _navigationStore.ProfileViewModel = profileViewModel;
            _navigationStore.CreateUserViewModel = createUserViewModel;
            _navigationStore.CreateRideViewModel = createRideViewModel;
            _navigationStore.RideDetailViewModel = rideDetailViewModel;
            _navigationStore.CurrentViewModel = loginViewModel;

            _navigationStore.CurrentViewModelChanged += OnCurrentViewModelChanged;

            mediator.Register<SelectedMessage<UserWrapper>>(async message => await UserSelected(message));
            mediator.Register<SelectedMessage<RideWrapper>>(message => ShowRideDetail(message));

            mediator.Register<UpdateMessage<RideWrapper>>(message => NavigateUpdateRideViewModel());
        }

        private void OnCurrentViewModelChanged()
        {
            OnPropertyChanged(nameof(CurrentViewModel));
        }

        private async Task UserSelected(SelectedMessage<UserWrapper> message)
        {
            if (message.Id != null)
            {
                var userDetail = await _userFacade.GetAsync(message.Id.Value);

                MenuViewModel.UserDetail = userDetail;
                ProfileViewModel.UserDetailModel = userDetail;
                _navigationStore.CurrentViewModel = RideViewModel;
                ProfileViewModel.Date_str = userDetail.Birthday.ToString("d", CultureInfo.GetCultureInfo("de-DE"));
            }
        }

        private void ShowRideDetail(SelectedMessage<RideWrapper> message)
        {
            _navigationStore.CurrentViewModel = RideDetailViewModel;
        }

        private void NavigateUpdateRideViewModel() => _navigationStore.CurrentViewModel = UpdateRideViewModel;

        public UserWrapper? Model { get; private set; }
        private readonly UserFacade _userFacade;
        public ViewModelBase CurrentViewModel => _navigationStore.CurrentViewModel;

        private readonly NavigationStore _navigationStore;
        public RideViewModel RideViewModel { get; }
        public RideDetailViewModel RideDetailViewModel { get; }
        public CreateUserViewModel CreateUserViewModel { get; }
        public ProfileViewModel ProfileViewModel { get; }
        public  UpdateRideViewModel UpdateRideViewModel { get; }
        public MenuViewModel MenuViewModel { get; set; }
    }
}   