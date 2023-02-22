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
using Microsoft.Xaml.Behaviors.Core;

namespace ICSProject.App.ViewModels
{
    public class CarsListViewModel : ViewModelBase, ICarsListViewModel
    {
        private readonly CarFacade _carFacade;
        private readonly IMediator _mediator;
        private readonly IMessageDialogService _messageDialogService;
        public ICommand CarEditedCommand { get; }
        public ICommand CarSelectedCommand { get; }


        public CarsListViewModel(CarFacade carFacade, IMediator mediator, MenuViewModel menuViewModel, IMessageDialogService messageDialogService)
        {
            _carFacade = carFacade;
            _mediator = mediator;
            MenuViewModel = menuViewModel;

            _messageDialogService = messageDialogService;
            
            CarEditedCommand = new AsyncRelayCommand(EditCar);
            CarSelectedCommand = new AsyncRelayCommand<CarListModel>(SelectedCar);
        }
        public MenuViewModel MenuViewModel { get; }

        public async Task DeleteAsync(CarListModel car)
        {
            var _car = await _carFacade.GetAsyncByCarId(car.Id);

            var delete = _messageDialogService.Show(
                "",
                $"Opravdu chcete smazat {_car.Manufacturer} {_car.Type}?",
                MessageDialogButtonConfiguration.YesNo,
                MessageDialogResult.No);

            if (delete == MessageDialogResult.No)
            {
                return;
            }

            await _carFacade.DeleteAsync(_car);
            await LoadAsync();
        }

        private async Task SelectedCar(CarListModel? car)
        {
            if(car == null) return;

            CarDetail = await _carFacade.GetAsyncByCarId(car.Id);
        }

        public async Task EditCar()
        {
            await _carFacade.SaveAsync(CarDetail);

            await LoadAsync();
        }

        public CarDetailModel CarDetail { get; set; }

        public ObservableCollection<CarListModel> Cars { get; set; } = new();


        public async Task LoadAsync()
        {
            var id = this.MenuViewModel.UserDetail.Id;//zjištění id pro filtrování pouze aut, která patří přihlášenému uživateli
            Cars.Clear();
            var cars = await _carFacade.GetAsync(id);
            Cars.AddRange(cars);
        }

    }
}