using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Threading.Tasks;
using ICSProject.App.ViewModels;
using ICSProject.BL.Models;
using ICSProject.App.Commands;
using ICSProject.BL.Facades;
using System.Windows;
using System.Threading.Tasks;
using System.Collections.ObjectModel;
using System.ComponentModel;
using ICSProject.App.Extensions;
using ICSProject.App.Services;

namespace ICSProject.App.ViewModels
{
    public class CreateRideViewModel : ViewModelBase, ICreateRideViewModel, ICarsListViewModel
    {
        private readonly RideFacade _rideFacade;
        private readonly CarFacade _carFacade;
        private readonly IMessageDialogService _messageDialogService;

        public CreateRideViewModel(
            MenuViewModel menuViewModel,
            RideFacade rideFacade,
            CarFacade carFacade,
            IMessageDialogService messageDialogService
        )
        {
            _carFacade = carFacade;
            _rideFacade = rideFacade;
            MenuViewModel = menuViewModel;
            CreateRideCommand = new AsyncRelayCommand(RideCreated);
            CreateSemiStopCommand = new RelayCommand(SemiStopCreated);
            _messageDialogService = messageDialogService;
        }

        public ICommand CreateRideCommand { get; }

        private string _rideStart = string.Empty;
        private string _rideEnd = string.Empty; 
        private Guid _selectedCarId = Guid.Empty;
        
        private string _rideTelephoneNumber = string.Empty;
        private string _rideHometown = string.Empty;
        private DateTime _rideStartTime = DateTime.Today;
        private DateTime _rideEndTime = DateTime.Today.AddDays(1.0);
        private uint _ridePrice = 0;

        public string RideStart
        {
            get
            {
                return _rideStart;
            }
            set
            {
                _rideStart = value;
                OnPropertyChanged(nameof(RideStart));
            }
        }
        public string RideEnd
        {
            get
            {
                return _rideEnd;
            }
            set
            {
                _rideEnd = value;
                OnPropertyChanged(nameof(RideEnd));
            }
        }
        public DateTime RideStartTime
        {
            get
            {
                return _rideStartTime;
            }
            set
            {
                _rideStartTime = value;
                OnPropertyChanged(nameof(RideStartTime));
            }
        }
        public DateTime RideEndTime
        {
            get
            {
                return _rideEndTime;
            }
            set
            {
                _rideEndTime = value;
                OnPropertyChanged(nameof(RideEndTime));
            }
        }
        public Guid SelectedCarId
        {
            get
            {
                return _selectedCarId;
            }
            set
            {
                _selectedCarId = value;
                OnPropertyChanged(nameof(SelectedCarId));
            }
        }
        public uint RidePrice
        {
            get
            {
                return _ridePrice;
            }
            set
            {
                _ridePrice = value;
                OnPropertyChanged(nameof(RidePrice));
            }
        }

        public ObservableCollection<CarListModel> Cars { get; set; } = new ();
        public ObservableCollection<SemiStopDetailModel> SemiStops { get; set; } = new();
        public SemiStopDetailModel SemiStop { get; set; }

        private string _semistopCity = string.Empty;
        private DateTime _semistopArrivalTime = DateTime.Today;
        private uint _semistopOrder = 1;

        public ICommand CreateSemiStopCommand { get; }

        public string SemistopCity
        {
            get
            {
                return _semistopCity;
            }
            set
            {
                _semistopCity = value;
                OnPropertyChanged(nameof(SemistopCity));
            }
        }

        public DateTime SemistopArrivalTime
        {
            get
            {
                return _semistopArrivalTime;
            }
            set
            {
                _semistopArrivalTime = value;
                OnPropertyChanged(nameof(SemistopArrivalTime));
            }
        }

        public uint SemistopOrder
        {
            get
            {
                return _semistopOrder;
            }
            set
            {
                _semistopOrder = value;
                
                _semistopOrder = OrderCheck(_semistopOrder);
                OnPropertyChanged(nameof(SemistopOrder));
            }
        }

        public uint OrderCheck(uint _semistopOrder)
        {
            foreach (var semi in SemiStops)
            {
                if (semi.Order == _semistopOrder)
                {
                    _semistopOrder += 1;
                    OrderCheck(_semistopOrder);
                }
            }

            return _semistopOrder;
        }

        //??
        private Visibility visibility = Visibility.Hidden;
        public Visibility Visibility
        {
            
            get
            {
                return visibility;
            }
            set
            {
                visibility = value;

                OnPropertyChanged("Visibility");
            }
        }

        public void SemiStopDeleted(SemiStopDetailModel semi)
        {
            SemiStops.Remove(SemiStops.Where(i => i.City == semi.City).Single());
        }

        private void SemiStopCreated()
        {
            if(SemistopCity == string.Empty || SemistopOrder < 1) return;

            var newSemiStop = new SemiStopDetailModel
            (
                City: SemistopCity,
                Order: SemistopOrder,
                ArrivalTime: SemistopArrivalTime,
                RideId: Guid.Empty
            );

            SemiStops.Add(newSemiStop);

            SemistopCity = string.Empty;
            SemistopOrder += 1;
            SemistopArrivalTime = DateTime.Today;
        }


        private async Task RideCreated()
        {
            var SelectedCarDetail = _carFacade.GetAsyncByCarId(SelectedCarId);

            //nevytvářej novou jízdu, pokud je nějaký z povinných údajů nevyplněný
            if (RideStart == string.Empty || RideEnd == string.Empty || SelectedCarId == Guid.Empty)
            {
                var __ = _messageDialogService.Show(
                    "Chyba!",
                    "Pro vytvoření spolujízdy musí být všecha políčka vyplněna!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return;
            }

            if (RideStartTime > RideEndTime)
            {
                var __ = _messageDialogService.Show(
                    "Chyba!",
                    "Začátek jízdy musí být dříve než konec!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return;
            }

            var newRide = new RideDetailModel
            (
                StarTime: RideStartTime,
                EndTime: RideEndTime,
                Start: RideStart,
                End: RideEnd,
                CarType: SelectedCarDetail.Result.Type,
                CarManufacturer: SelectedCarDetail.Result.Manufacturer,
                CarNumberOfSeats: SelectedCarDetail.Result.NumberOfSeats,
                CarId: SelectedCarId,
                Price: RidePrice,
                DriverId: this.MenuViewModel.UserDetail.Id
            )
            {
                //CarImageUrl = SelectedCarDetail.Result.ImageUrl;
            };

            //tady přidat seznam mezizastavek
            newRide.SemiStops.AddRange(SemiStops);
            //propojeni mezizastavek s jízdou
            foreach (var sem in newRide.SemiStops)
            {
                sem.RideId = newRide.Id;
            }

            await _rideFacade.SaveAsync(newRide);

            //vynulování textblocků po přidání nového uživatele
            RideStart = string.Empty;
            RideEnd = string.Empty;
            RidePrice = 0;
            SemiStops.Clear();

            var _ = _messageDialogService.Show(
                "Přidání jízdy",
                "Nová spolujízda úspěšně vytvořena!",
                MessageDialogButtonConfiguration.OK,
                MessageDialogResult.OK);
        }

        public async Task LoadAsync()
        {
            var id = this.MenuViewModel.UserDetail.Id;//zjištění id pro filtrování pouze aut, která patří přihlášenému uživateli
            Cars.Clear();
            var cars = await _carFacade.GetAsync(id);
            Cars.AddRange(cars);
        }

        public MenuViewModel MenuViewModel { get; set; }
    }
}
