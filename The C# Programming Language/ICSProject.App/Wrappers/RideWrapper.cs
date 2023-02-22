using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using ICSProject.App.Extensions;
using ICSProject.BL.Models;

namespace ICSProject.App.Wrappers
{
    public class RideWrapper : ModelWrapper<RideDetailModel>
    {
        public RideWrapper(RideDetailModel model) : base(model)
        {
            InitializeCollectionProperties(model);
        }

        public DateTime StarTime
        {
            get => GetValue<DateTime>();
            set => SetValue(value);
        }
        public DateTime EndTime
        {
            get => GetValue<DateTime>();
            set => SetValue(value);
        }
        public string? Start
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? End
        {
            get => GetValue<string>();
            set => SetValue(value);
        }

        public string? CarType
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? CarManufacturer
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public uint? CarNumberOfSeats
        {
            get => GetValue<uint>();
            set => SetValue(value);
        }

        public Guid DriverId
        {
            get => GetValue<Guid>();
            set => SetValue(value);
        }
        public Guid CarId
        {
            get => GetValue<Guid>();
            set => SetValue(value);
        }
        public string? CarImageUrl
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public uint? Price
        {
            get => GetValue<uint>();
            set => SetValue(value);
        }

        private void InitializeCollectionProperties(RideDetailModel model)
        {
            if (model.PassengerRides == null)
            {
                throw new ArgumentException("PassengerRides cannot be null");
            }
            PassengerRides.AddRange(model.PassengerRides.Select(e => new PassengerRideWrapper(e)));

            RegisterCollection(PassengerRides, model.PassengerRides);

            if (model.SemiStops == null)
            {
                throw new ArgumentException("SemiStops cannot be null");
            }
            SemiStops.AddRange(model.SemiStops.Select(e => new SemiStopWrapper(e)));

            RegisterCollection(SemiStops, model.SemiStops);
        }

        public ObservableCollection<PassengerRideWrapper> PassengerRides { get; set; } = new();

        public ObservableCollection<SemiStopWrapper> SemiStops { get; set; } = new();

        public static implicit operator RideWrapper(RideDetailModel detailModel)
            => new(detailModel);

        public static implicit operator RideDetailModel(RideWrapper wrapper)
            => wrapper.Model;
    }
}