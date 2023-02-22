using AutoMapper;
using ICSProject.DAL.Entities;

namespace ICSProject.BL.Models
{
    public record SemiStopDetailModel(
        string City,
        uint Order,
        DateTime ArrivalTime,
        Guid RideId) : ModelBase
    {
        public string City { get; set; } = City;
        public uint Order { get; set; } = Order;
        public DateTime ArrivalTime { get; set; } = ArrivalTime;
        public Guid RideId { get; set; } = RideId;

        public class MapperProfile : Profile
        {
            public MapperProfile()
            {
                CreateMap<SemiStopEntity, SemiStopDetailModel>()
                    .ReverseMap();
            }
        }

        public static SemiStopDetailModel Empty => new (string.Empty, 0, default, default);
    }
}