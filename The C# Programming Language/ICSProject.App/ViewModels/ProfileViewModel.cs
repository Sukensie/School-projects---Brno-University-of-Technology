using System;
using ICSProject.App.Commands;
using ICSProject.App.Extensions;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using System.Collections.ObjectModel;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;
using ICSProject.App.Services;
using ICSProject.App.Views;
using Microsoft.EntityFrameworkCore.Metadata.Internal;

namespace ICSProject.App.ViewModels
{
    public class ProfileViewModel : ViewModelBase, IProfileViewModel, IListViewModel
    {
        private readonly UserFacade _userFacade;
        private readonly RideFacade _rideFacade;
        private readonly IMessageDialogService _messageDialogService;

        public ProfileViewModel(
            MenuViewModel menuViewModel,
            UserFacade userFacade,
            RideFacade rideFacade,
            IMessageDialogService messageDialogService
        )
        {
            _userFacade = userFacade;
            _rideFacade = rideFacade;
            MenuViewModel = menuViewModel;
            UserEditCommand = new AsyncRelayCommand(UserEdited);

            RideDeleteCommand = new AsyncRelayCommand(DeleteRide);

            RideSelectedCommand = new RelayCommand<RideDetailModel>(RideSelected);

            _messageDialogService = messageDialogService;
        }
        public async Task UserEdited()
        {
            await _userFacade.SaveAsync(UserDetailModel);
            await MenuViewModel.LoadAsync();
            await LoadAsync();
        }

        public ICommand UserEditCommand { get; }

        public ICommand RideDeleteCommand { get; }
        public ICommand RideSelectedCommand { get; }

        public UserDetailModel UserDetailModel { get; set; }

        public ObservableCollection<RideDetailModel> Rides { get; set; } = new();

        private string _selectedTypeFilter = string.Empty;

        public string SelectedTypeFilter
        {
            get
            {
                return _selectedTypeFilter;
            }
            set
            {
                _selectedTypeFilter = value;
                OnPropertyChanged(nameof(SelectedTypeFilter));
                LoadAsync();
            }
        }
        
        public string Date_str { get; set; }      

        public MenuViewModel MenuViewModel { get; set; }

        public Guid RideId = Guid.Empty;

        private void RideSelected(RideDetailModel? ride)
        {
            if (ride == null) return;
            //Po kliknuti na polozku si zde udrzim jeji ID
            RideId = ride.Id;
        }

        public async Task DeleteRide()
        {
            if (RideId == Guid.Empty) return;

            var _ride = await _rideFacade.GetAsync(RideId);

            var delete = _messageDialogService.Show(
                "",
                $"Opravdu chcete smazat jízdu {_ride.Start} -> {_ride.End}?",
                MessageDialogButtonConfiguration.YesNo,
                MessageDialogResult.No);

            if (delete == MessageDialogResult.No)
            {
                return;
            }

            if (_ride.DriverId != MenuViewModel.UserDetail.Id)
            {
                return;
            }

            _ride.PassengerRides.Clear();
            await _rideFacade.SaveAsync(_ride);
            _ride = await _rideFacade.GetAsync(RideId);
            await _rideFacade.DeleteAsync(_ride);
            RideId = Guid.Empty;
            await LoadAsync();
        }

        public async Task LoadAsync()
        {
            Rides.Clear();
            var filter = SelectedTypeFilter;
            //var RideDetailModel rides;
            var id = this.MenuViewModel.UserDetail.Id;//zjištění id pro filtrování pouze aut, která patří přihlášenému uživateli
            if(filter == "System.Windows.Controls.ComboBoxItem: Spolujezdec")
            {
                Rides.Clear();
                var rides = await _rideFacade.GetAsyncPassengerById(id);
                var ridesArray = rides.ToArray();
                foreach(var ride in ridesArray)
                {
                    var rides2 = await _rideFacade.GetAsyncByRideId(ride.RideId);
                    Rides.AddRange(rides2);
                }
            }
            else
            {
                var rides = await _rideFacade.GetAsyncById(id);
                Rides.Clear(); //fsr to loopuje 3x, ale když ten clear je až za getAsync, tak to ukazuje korektní počet záznamů
                Rides.AddRange(rides);
            }
        }
    }
}
