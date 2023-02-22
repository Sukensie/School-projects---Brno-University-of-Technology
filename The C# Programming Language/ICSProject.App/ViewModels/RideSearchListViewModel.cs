using System;
using System.ComponentModel;
using System.Text;
using System.Windows.Input;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Threading.Tasks;
using ICSProject.App.Extensions;
using ICSProject.App.Services;
using ICSProject.BL.Facades;
using ICSProject.BL.Models;
using System.Windows.Data;
using ICSProject.App.Commands;
using ICSProject.App.Messages;
using ICSProject.App.Wrappers;

namespace ICSProject.App.ViewModels
{
    public class RideSearchListViewModel : ViewModelBase, IRideSearchListViewModel
    {
        private readonly ObservableCollection<RideViewModel> _RideViewModels;
        private readonly RideFacade _rideFacade;
        private readonly UserFacade _userFacade;
        public RideDetailViewModel RideDetailViewModel { get; set; }
        public ICollectionView RidesCollectionView { get; }

        public IMediator Mediator { get; set; }

        public ICommand RidesSearchCommand { get; }
        public ICommand RideSelectedCommand { get; }

        private string _ridesFilterStart = string.Empty;
        private string _ridesFilterEnd = string.Empty;
        private DateTime _ridesFilterDate;

        public RideSearchListViewModel(RideFacade rideFacade, UserFacade userFacade, RideDetailViewModel rideDetailViewModel, IMediator mediator)
        {
            _RideViewModels = new ObservableCollection<RideViewModel>();
            _rideFacade = rideFacade;
            _userFacade = userFacade;

            Mediator = mediator;
            RidesCollectionView = CollectionViewSource.GetDefaultView(_RideViewModels);

            RideSelectedCommand = new AsyncRelayCommand<RideListModel>(ShowRideDetail);
            RidesSearchCommand = new RelayCommand<RideListModel>(RidesSearched);

            RideDetailViewModel = rideDetailViewModel;
        }

        public string RidesFilterStart
        {
            get
            {
                return _ridesFilterStart;
            }
            set
            {
                _ridesFilterStart = value;
                OnPropertyChanged(nameof(RidesFilterStart));
                RidesCollectionView.Refresh();
            }
        }
        public string RidesFilterEnd
        {
            get
            {
                return _ridesFilterEnd;
            }
            set
            {
                _ridesFilterEnd = value;
                OnPropertyChanged(nameof(RidesFilterEnd));
                RidesCollectionView.Refresh();
            }
        }

        public DateTime RidesFilterDate
        {
            get
            {
                return _ridesFilterDate;
            }
            set
            {
                _ridesFilterDate = value;
                OnPropertyChanged(nameof(RidesFilterDate));
                RidesCollectionView.Refresh();
            }
        }

        public int? Seats { get; set; }


        private async Task ShowRideDetail(RideListModel? rideListModel)
        {
            if (rideListModel == null)
            {
                return;
            }

            Mediator.Send(new SelectedMessage<RideWrapper> {Id = rideListModel.Id});
        }

        private void RidesSearched(RideListModel? rideListModel)
        {
            LoadAsync();
        }

        public ObservableCollection<RideListModel> Rides { get; } = new();
        public async Task LoadAsync()
        {
            var rides = await _rideFacade.GetAsync(RidesFilterStart, RidesFilterEnd, RidesFilterDate);
            Rides.Clear();
            Rides.AddRange(rides);

            foreach (var ride in Rides)
            {
                ride.SeatsLeft = Convert.ToInt32(ride.CarNumberOfSeats) - Convert.ToInt32(ride.PassengerRides.Count) - 1;
            }
        }
    }
}
