﻿using System.Windows;

namespace ICSProject.App.Services
{
    public class MessageDialogService : IMessageDialogService
    {
        public MessageDialogResult Show(
            string title, 
            string caption, 
            MessageDialogButtonConfiguration buttonConfiguration, 
            MessageDialogResult defaultResult)
        {
            var messageDialog = new MessageDialog(title, caption, defaultResult, buttonConfiguration)
            {
                Owner = Application.Current.MainWindow
            };
            return messageDialog.ShowDialog();
        }
    }
}