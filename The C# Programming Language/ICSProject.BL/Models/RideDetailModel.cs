using System;
using ICSProject.DAL.Entities;
using AutoMapper;

namespace ICSProject.BL.Models
{
    public record RideDetailModel(
        DateTime StarTime,
        DateTime EndTime,
        string Start,
        string End,
        string CarType,
        string CarManufacturer,
        uint CarNumberOfSeats,
        Guid DriverId,
        Guid CarId,
        uint Price) : ModelBase
    {
        public DateTime StarTime { get; set; } = StarTime;
        public DateTime EndTime { get; set; } = EndTime;
        public string Start { get; set; } = Start;
        public string End { get; set; } = End;
        public List<SemiStopDetailModel> SemiStops { get; init; } = new();
        public string CarType { get; set; } = CarType;
        public string CarManufacturer { get; set; } = CarManufacturer;
        public uint CarNumberOfSeats { get; set; } = CarNumberOfSeats;
        public Guid DriverId { get; set; } = DriverId;
        public Guid CarId { get; set; } = CarId;
        public string? CarImageUrl { get; set; }
        public uint Price { get; set; } = Price;
        public List<PassengerRideDetailModel> PassengerRides { get; init; } = new();
       //public string Passengers { get; set; } = Passengers;


        public class MapperProfile : Profile
        {
            
            public MapperProfile()
            {
                CreateMap<RideEntity, RideDetailModel>()
                    .ReverseMap()
                    .ForMember(entity => entity.Car, expression => expression.Ignore());
                
            }
        }

        public static RideDetailModel Empty => new(default, default , string.Empty, string.Empty, string.Empty, string.Empty, 0, default, default, 0);
    }
}