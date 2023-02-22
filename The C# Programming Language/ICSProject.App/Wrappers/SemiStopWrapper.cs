using System;
using System.Collections.Generic;
using ICSProject.BL.Models;

namespace ICSProject.App.Wrappers
{
    public class SemiStopWrapper : ModelWrapper<SemiStopDetailModel>
    {
        public SemiStopWrapper(SemiStopDetailModel model) : base(model) { }

        public string? City
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public DateTime ArrivalTime
        {
            get => GetValue<DateTime>();
            set => SetValue(value);
        }
        public uint? Order
        {
            get => GetValue<uint>();
            set => SetValue(value);
        }
        public Guid? RideId
        {
            get => GetValue<Guid>();
            set => SetValue(value);
        }

        public static implicit operator SemiStopWrapper(SemiStopDetailModel detailModel)
            => new(detailModel);

        public static implicit operator SemiStopDetailModel(SemiStopWrapper wrapper)
            => wrapper.Model;
    }
}