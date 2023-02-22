using AutoMapper;
using ICSProject.DAL.Entities;

using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Windows;
using System;
using ICSProject.BL.Models;

namespace ICSProject.App.Wrappers
{
    public class PassengerRideWrapper : ModelWrapper<PassengerRideDetailModel>
    {
        public PassengerRideWrapper(PassengerRideDetailModel model) : base(model) { }

        public Guid PassengerId
        {
            get => GetValue<Guid>();
            set => SetValue(value);
        }
        public Guid RideId
        {
            get => GetValue<Guid>();
            set => SetValue(value);
        }
        public string? PassengerName
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        public string? PassengerImageUrl
        {
            get => GetValue<string>();
            set => SetValue(value);
        }
        
        public static implicit operator PassengerRideWrapper(PassengerRideDetailModel detailModel)
            => new(detailModel);

        public static implicit operator PassengerRideDetailModel(PassengerRideWrapper wrapper)
            => wrapper.Model;
    }
}