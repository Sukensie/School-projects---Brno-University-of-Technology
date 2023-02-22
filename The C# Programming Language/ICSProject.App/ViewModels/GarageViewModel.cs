using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Threading.Tasks;
using ICSProject.App.Services;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using ICSProject.App.Commands;
using ICSProject.App.Messages;
using ICSProject.App.Wrappers;

namespace ICSProject.App.ViewModels
{
    public class GarageViewModel : ViewModelBase
    {
        private readonly CarFacade _carFacade;
        private readonly IMessageDialogService _messageDialogService;

        public ICommand CreateCarCommand { get; }

        private readonly IMediator _mediator;
        public GarageViewModel(
            CarsListViewModel carsListViewModel,
            IMediator mediator,
            MenuViewModel menuViewModel,
            CarFacade carFacade,
            IMessageDialogService messageDialogService)
        {
            MenuViewModel = menuViewModel;
            _carFacade = carFacade;
            CarsListViewModel = carsListViewModel;
            _mediator = mediator;

            _messageDialogService = messageDialogService;

            CreateCarCommand = new AsyncRelayCommand(CarCreated);
        }

        private string _carManufacturer = string.Empty;
        private string _carType = string.Empty;
        private string _carImageUrl = string.Empty;
        private uint _carNumberOfSeats = uint.MinValue;
        private DateTime _carRegistrationTime = DateTime.Today;
        public string CarManufacturer
        {
            get
            {
                return _carManufacturer;
            }
            set
            {
                _carManufacturer = value;
                OnPropertyChanged(nameof(CarManufacturer));
            }
        }
        public string CarType
        {
            get
            {
                return _carType;
            }
            set
            {
                _carType = value;
                OnPropertyChanged(nameof(CarType));
            }
        }
        public string CarImageUrl
        {
            get
            {
                return _carImageUrl;
            }
            set
            {
                _carImageUrl = value;
                OnPropertyChanged(nameof(CarImageUrl));
            }
        }
        public uint CarNumberOfSeats
        {
            get
            {
                return _carNumberOfSeats;
            }
            set
            {
                _carNumberOfSeats = value;
                OnPropertyChanged(nameof(CarNumberOfSeats));
            }
        }
        public DateTime CarRegistrationTime
        {
            get
            {
                return _carRegistrationTime;
            }
            set
            {
                _carRegistrationTime = value;
                OnPropertyChanged(nameof(CarRegistrationTime));
            }
        }

        public async Task CarCreated()
        {
            //nevytvářej nové auto, pokud je nějaký z povinných údajů nevyplněný
            if (CarManufacturer == string.Empty || CarType == string.Empty || CarNumberOfSeats == 0)
            {

                var __ = _messageDialogService.Show(
                    "Chyba!",
                    "Pro přidání auta musí být všecha políčka (vyjma URL fotky) vyplněna!",
                    MessageDialogButtonConfiguration.OK,
                    MessageDialogResult.OK);
                return;
            }
        
            if (CarImageUrl == string.Empty)
            {
                CarImageUrl = "/Resources/Images/DefaultVehicle.png";
            }

            var newCar = new CarDetailModel
            (
                Manufacturer: CarManufacturer,
                Type: CarType,
                RegistrationTime: CarRegistrationTime,
                NumberOfSeats: CarNumberOfSeats,
                ImageUrl: CarImageUrl,
                OwnerId: this.MenuViewModel.UserDetail.Id
            )
            {
                //Id = příprava na editování, tam budu potřebovat ID aby se nevytvořilo nové ale updatnulo staré viz BL testing 
            };

            await _carFacade.SaveAsync(newCar);

            //vynulování textblocků po přidání nového auta
            CarManufacturer = string.Empty;
            CarType = string.Empty;
            CarImageUrl = string.Empty;
            CarNumberOfSeats = uint.MinValue;
            CarRegistrationTime = DateTime.MinValue;

            await CarsListViewModel.LoadAsync();

            var _ = _messageDialogService.Show(
                "Nové auto",
                $"{newCar.Manufacturer} {newCar.Type} byl úspěšně přidán!",
                MessageDialogButtonConfiguration.OK,
                MessageDialogResult.OK);

        }

        public CarsListViewModel CarsListViewModel { get; }
        public MenuViewModel MenuViewModel { get; }
    }
}
