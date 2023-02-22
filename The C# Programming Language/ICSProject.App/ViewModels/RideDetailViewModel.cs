using System;
using System.ComponentModel;
using System.Text;
using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Controls;
using ICSProject.App.Extensions;
using ICSProject.App.Services;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using System.Windows.Data;
using ICSProject.App.Commands;
using ICSProject.App.Messages;
using ICSProject.App.Wrappers;
using MenuItem = ICSProject.App.Wrappers.MenuItem;

namespace ICSProject.App.ViewModels
{
    public class RideDetailViewModel : ViewModelBase
    {
        public RideDetailViewModel(RideFacade rideFacade, UserFacade userFacade, MenuViewModel menuViewModel, IMediator mediator
            , IMessageDialogService messageDialogService)
        {
            MenuViewModel = menuViewModel;
            Mediator = mediator;
            _messageDialogService = messageDialogService;
            _rideFacade = rideFacade;
            _userFacade = userFacade;

            Mediator.Register<SelectedMessage<RideWrapper>>(async message => await InitModel(message));
            JoinRideCmd = new AsyncRelayCommand(JoinRide, CanJoin);
            EditRideCmd = new RelayCommand(EditRide, CanEdit);
            PassengerSelectedCommand = new RelayCommand<PassengerRideWrapper>(PassengerSelected);
            PassengerDeleteCmd= new AsyncRelayCommand(PassengerDelete);
        }

        public async Task InitModel(SelectedMessage<RideWrapper> message)
        {
            RideId = message.Id;
            await LoadAsync();
        }

        public async Task LoadAsync()
        {
            if (RideId == null)
            {
                return;
            }

            Ride = await _rideFacade.GetAsync((Guid)RideId);
            Driver = await _userFacade.GetAsync(Ride.DriverId);
            DriverId = Driver.Id;

            StartDate = Ride.StarTime.ToLongDateString();

            RideInfo = Driver?.Name + "  " + Ride.Start + " - " + Ride.End;
            RideDestinations = new();

            RideDestinations.Add(new RideDestination(Ride.Start, Ride.StarTime.ToShortTimeString()));

            foreach (var semistop in Ride.SemiStops)
            {
                RideDestinations.Insert(Convert.ToInt32(semistop.Order), new RideDestination(semistop.City, semistop.ArrivalTime.ToShortTimeString()));
            }

            RideDestinations.Add(new RideDestination(Ride.End, Ride.EndTime.ToShortTimeString()));
            SeatsLeft = Convert.ToInt32(Ride.CarNumberOfSeats) - Ride.PassengerRides.Count - 1;
        }

        public async Task SaveAsync()
        {
            Ride = await _rideFacade.SaveAsync(Ride);
            RideId = Ride.Id;
        }

        private bool CanJoin()
        {
            if (Driver is null || MenuViewModel is null || Ride is null)
            {
                return false;
            }

            if (Driver.Id == MenuViewModel.UserDetail.Id)
            {
                _messageDialogService.Show(
                    "Chyba",
                    "Účastník je řidičem této jízdy!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return false;
            }

            if (Convert.ToInt32(Ride.CarNumberOfSeats) <= Ride.PassengerRides.Count + 1)
            {
                _messageDialogService.Show(
                    "Chyba",
                    "Jízda je již plná!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return false;
            }

            foreach (var p in Ride.PassengerRides)
            {
                if (p.PassengerId == MenuViewModel.UserDetail.Id)
                {
                    _messageDialogService.Show(
                        "Chyba",
                        "Účastník se již v této jízdě nachází!",
                        MessageDialogButtonConfiguration.OK,
                        MessageDialogResult.OK);
                    return false;
                }   
            }

            return true;
        }

        public async Task JoinRide()
        {
            var passengerDetail = new PassengerRideDetailModel(
                MenuViewModel.UserDetail.Id,
                Ride.Id,
                MenuViewModel.UserDetail.Name,
                MenuViewModel.UserDetail.ImageUrl);

            if (Ride == null)
            {
                return;
            }

            var _rides = await _rideFacade.GetAsync();
            Rides.AddRange(_rides);
            foreach (var r in Rides)
            {
                foreach (var p in r.PassengerRides)
                {
                    if (p.PassengerId == MenuViewModel.UserDetail.Id && ((r.StarTime <= Ride.EndTime) && (r.EndTime >= Ride.StarTime)))
                    {
                        _messageDialogService.Show(
                            "Chyba",
                            "Účastník se nachází na jiné jízdě se stejným časem",
                            MessageDialogButtonConfiguration.OK,
                            MessageDialogResult.OK);
                        return;
                    }   
                }
            }
            Ride.PassengerRides.Add(passengerDetail);
            Rides.Clear();
            await SaveAsync();
            await LoadAsync();
        }

        private bool CanEdit()
        {
            if (Driver == null || MenuViewModel == null)
            {
                return false;
            }

            if (Driver.Id != MenuViewModel.UserDetail.Id)
            {
                _messageDialogService.Show(
                    "Chyba",
                    "Nemáte dostatečná oprávnění na tuto akci!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.No);
                return false;
            }

            return true;
        }

        public void EditRide()
        {
            Mediator.Send(new UpdateMessage<RideWrapper> {Id = RideId});
        }

        public void PassengerSelected(PassengerRideWrapper? passenger)
        {
            if (passenger is null ) return;
            
            PassengerViewSelection = passenger.Id;
        }

        public async Task PassengerDelete()
        {
            if (Driver == null || MenuViewModel == null || Ride == null || PassengerViewSelection == Guid.Empty)
            {
                return;
            }

            Guid userid = Ride.PassengerRides.FirstOrDefault(p => p.Id == PassengerViewSelection)!.PassengerId;

            if (userid != MenuViewModel.UserDetail.Id && MenuViewModel.UserDetail.Id != Driver.Id)
            {
                _messageDialogService.Show(
                    "Chyba",
                    "Nemáte dostatečná oprávnění na tuto akci!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.No);
                return;
            }

            var delete = _messageDialogService.Show(
                "",
                "Opravdu chcete odebrat spolujezdce??",
                MessageDialogButtonConfiguration.YesNo,
                MessageDialogResult.No);

            if (delete == MessageDialogResult.No)
            {
                return;
            }

            foreach (var passenger in Ride.PassengerRides)
            {
                if (passenger.Id == PassengerViewSelection)
                {
                    Ride.PassengerRides.Remove(passenger);
                    await SaveAsync();
                    break;
                }
            }
            PassengerViewSelection = Guid.Empty;
            await LoadAsync();
        }

        private Guid PassengerViewSelection = Guid.Empty;

        private bool EditMode { get; set; }

        public ICommand JoinRideCmd { get; set; }
        public ICommand EditRideCmd { get; set; }
        public ICommand PassengerSelectedCommand { get; set; }
        public ICommand PassengerDeleteCmd { get; set; }

        public Guid? RideId { get; set; }
        public Guid? DriverId { get; set; }

        public RideWrapper? Ride { get; set; }
        public UserDetailModel? Driver { get; set; }
        public string? RideInfo { get; set; }
        public string? StartDate { get; set; }
        public MenuViewModel? MenuViewModel { get; }
        public IMediator Mediator { get; }
        public ObservableCollection<RideDestination>? RideDestinations { get; set; }
        public ObservableCollection<RideListModel>? Rides = new();
        public int? SeatsLeft { get; set; }
        private readonly RideFacade _rideFacade;
        private readonly UserFacade _userFacade;

        private readonly IMessageDialogService _messageDialogService;
    }
}
