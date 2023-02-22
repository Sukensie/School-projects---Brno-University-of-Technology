using System;
using System.Collections.ObjectModel;
using System.Threading.Tasks;
using System.Windows.Input;

using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using ICSProject.App.Extensions;
using ICSProject.App.Services;
using ICSProject.App.Messages;
using ICSProject.App.Wrappers;
using ICSProject.App.Commands;
using ICSProject.App.Views;

namespace ICSProject.App.ViewModels
{
    public class UsersListViewModel : ViewModelBase, IUsersListViewModel
    {
        private readonly UserFacade _userFacade;
        private readonly RideFacade _rideFacade;
        private readonly IMediator _mediator;
        private readonly IMessageDialogService _messageDialogService;

        public UsersListViewModel(UserFacade userFacade, IMediator mediator, IMessageDialogService messageDialogService, RideFacade rideFacade)
        {
            _userFacade = userFacade;
            _mediator = mediator;
            _rideFacade = rideFacade;

            _messageDialogService = messageDialogService;

            UserSelectedCommand = new RelayCommand<UserListModel>(UserSelected);

            UserSearchedCommand = new RelayCommand<UserListModel>(UserSearched);

            UserDeletedCommand = new AsyncRelayCommand(DeleteUser);

            UserLogInCommand = new RelayCommand(LogIn);
        }

        private string _userSelect = string.Empty;

        public string UserSelect
        {
            get
            {
                return _userSelect;
            }
            set
            {
                _userSelect = value;
                OnPropertyChanged(nameof(UserSelect));
            }
        }

        public ICommand UserSelectedCommand { get; }

        public ICommand UserSearchedCommand { get; }

        public ICommand UserDeletedCommand { get; }

        public ICommand UserLogInCommand { get; }


        public async Task DeleteUser()
        {
            //pokud neni nic zvoleneho, nic nedelej
            if(UserID == Guid.Empty) return;

            if (Users.Count == 1)
            {
                var _ = _messageDialogService.Show(
                    "Chyba!",
                    $"Alespoň jeden uživatel by měl zůstat!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return;
            }

            //jinak nacti uzivatele a smaz ho z databaze
            var _usr = await _userFacade.GetAsync(UserID);

            var delete = _messageDialogService.Show(
                "",
                $"Opravdu chcete smazat uživatele {_usr.Name}?",
                MessageDialogButtonConfiguration.YesNo,
                MessageDialogResult.No);

            if (delete == MessageDialogResult.No)
            {
                return;
            }

            //smazat jizdy uzivatele, ktere ridi a jsou v nich spolujezdci
            Rides.Clear();
            var _rides = await _rideFacade.GetAsync();
            Rides.AddRange(_rides);
            foreach (var _r in Rides)
            {
                var _ride = await _rideFacade.GetAsync(_r.Id);
                if (_ride.DriverId == _usr.Id)
                {
                    _ride.PassengerRides.Clear();
                    await _rideFacade.SaveAsync(_ride);
                    _ride = await _rideFacade.GetAsync(_ride.Id);
                    await _rideFacade.DeleteAsync(_ride);
                }
            }

            await _userFacade.DeleteAsync(_usr);
            await LoadAsync();
        }

        public Guid UserID = Guid.Empty;
        public ObservableCollection<RideListModel> Rides { get; } = new();

        private void UserSelected(UserListModel? user)
        {
            if(user == null) return;
            //Po kliknuti na polozku si zde udrzim jeji ID
            UserID = user.Id;
        }

        private void LogIn()
        {
            //Pokud neni zadna polozka zvolena, nic se nestane
            if(UserID == Guid.Empty) return;
            //jinak nacti profil zvoleneho uzivatele
            _mediator.Send(new SelectedMessage<UserWrapper> {Id = UserID});
        }

        private void UserSearched(UserListModel? user)
        {
            LoadAsync();
        }

        public ObservableCollection<UserListModel> Users { get; } = new();

        public async Task LoadAsync()
        {
            Users.Clear();
            var users = await _userFacade.GetAsync(UserSelect);
            Users.AddRange(users);
        }
    }
}